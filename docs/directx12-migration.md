# DirectX 12 migration

## Current architecture

The client now creates a native `ID3D12Device` and direct
`ID3D12CommandQueue`. The existing renderer is connected to those objects
through the Windows D3D9On12 translation layer. There is no fallback to a
native Direct3D 9 device: successful graphics initialization means commands
are executed by DirectX 12.

`CDirectX12Backend` owns the native DirectX 12 objects. This keeps their
lifetime separate from the legacy rendering facade and provides one boundary
from which renderer subsystems can be moved to native command lists.

## Why the transition layer is necessary

The original renderer contains hundreds of direct D3D9 calls and relies on:

- fixed-function render and texture-stage state;
- D3D9 resource locking and implicit synchronization;
- D3D9-era shader bytecode and vertex declarations;
- implicit resource transitions and device-loss handling;
- additional D3D9 swap chains for windowed viewports.

DirectX 12 has no equivalent fixed-function pipeline and requires explicit
pipeline state objects, descriptors, resource barriers, command recording,
fences, and residency management. D3D9On12 keeps the game renderable while
those responsibilities are migrated independently.

## Native migration boundary

The next native subsystems should depend on `CDirectX12Backend` rather than
accessing global COM objects:

1. ~~frame contexts, command allocators, command lists, and fences;~~
2. ~~render-target interoperability and native swap-chain cutover;~~
3. ~~upload heaps and static vertex/index buffers;~~
4. ~~descriptor heaps and texture uploads;~~
5. fixed-function state replacement with HLSL and cached PSOs;
6. removal of D3DX9 and, finally, D3D9On12.

Each stage should land with a visual regression test before the following
stage replaces more of the compatibility renderer.

### Stage 1: frame execution infrastructure

Completed. The backend owns three reusable frame contexts, one direct command
list, and a timeline fence. A frame waits only when its allocator is about to
be reused. Native command submission is ordered after D3D9On12 presentation on
the shared direct queue, and shutdown performs an explicit GPU drain.

### Stage 2a: render-target interoperability

Completed. `CDirectX12RenderTargetManager` checks the active D3D9 render target
out of the translation layer, exposes its underlying `ID3D12Resource`, creates
one RTV descriptor per in-flight frame, and records the required
`COMMON -> RENDER_TARGET -> COMMON` transitions. The frame fence is passed
back to D3D9On12 before `Present`, so the translation layer cannot reuse the
resource before native work finishes.

The visible replacement path now copies the completed shared backbuffer into
an `IDXGISwapChain3` owned by `CDirectX12PresentationManager`. In `replace`
mode DXGI performs the visible `Present` and the legacy D3D9 swap-chain
presentation is bypassed. Shadow, overlay, and split modes retain the legacy
presentation path for diagnostics.

### Stage 3: upload heaps and GPU buffers

Completed as native infrastructure. `CDirectX12UploadManager` owns growable,
persistently mapped upload pages for every in-flight frame. A page is reset
only after the backend has waited for that frame's fence, which avoids both
per-upload committed allocations and writes to memory still consumed by the
GPU.

`CDirectX12Buffer` owns DEFAULT-heap vertex or index resources, tracks their
current resource state, records repeatable CPU-to-GPU uploads, and exposes the
corresponding `D3D12_VERTEX_BUFFER_VIEW` or `D3D12_INDEX_BUFFER_VIEW`.
It supports the renderer's existing separate vertex streams and both 16-bit
and 32-bit index formats.

The existing D3D9 buffers remain active until a native draw path has root
signatures and pipeline state objects. Replacing them earlier would allocate
duplicate geometry without providing a D3D12 consumer. The next stage is the
first complete native draw path that can consume both the new buffers and
texture descriptors.

### Stage 4: descriptors and texture uploads

Completed as native infrastructure. The backend owns separate shader-visible
heaps for CBV/SRV/UAV resources and samplers, binds them when each native
command list begins, and exposes reusable descriptor allocation rather than
embedding heap offsets in texture code.

`CDirectX12Texture` creates DEFAULT-heap 2D resources, owns a persistent SRV,
tracks resource state, and uploads one or more mip levels through the
per-frame upload manager. Uploads use `GetCopyableFootprints`, so row pitch,
placement alignment, and block-compressed BC1/BC2/BC3 data follow the D3D12
copy requirements.

`GetDirectX12TextureFormat` maps the legacy ARGB, luminance, alpha, and
DXT formats to DXGI. Luminance and alpha formats receive explicit component
mapping in their SRV. The 4-bit `A4R4G4B4` and `X4R4G4B4` formats are
expanded on the CPU to BGRA8 before upload because D3D12 does not expose the
legacy layout directly. Legacy 24-bit RGB still requires a conversion path.

D3D9 textures remain the active rendering resources until the first native
root signature and PSO can consume these SRVs. That first complete native draw
path is the next stage.

### Stage 5a: first complete native draw path

Completed. `CDirectX12NativeRenderer` records an indexed, textured draw after
the D3D9 render target is unwrapped and before it is returned for `Present`.
The validation pass uses the native vertex/index buffers, SRV heap, sampler
heap, root signature, HLSL vertex/pixel shaders, PSO, viewport, scissor, and
render-target view as one end-to-end path.

The pass is intentionally transparent and restricted to one pixel, so it
executes the native pipeline without changing the visible game frame or
adding a full-screen bandwidth cost. Its resources are uploaded lazily on the
first frame and then reused.

`CDirectX12PipelineCache` compiles the transitional Shader Model 5.1 HLSL once
and caches PSOs by render-target format and multisample description. Runtime
compilation avoids adding a new DXC runtime dependency during the transition;
the production shader set should move to offline DXC compilation when native
draw paths replace visible D3D9 subsystems.

This does not complete the full fixed-function replacement in roadmap item 5.
The next migration should move a visible but bounded subsystem, such as the
2D `DrawPort` primitives, onto reusable native pipelines and compare its
output against the compatibility renderer.

### Stage 5b: DrawPort solid-primitive shadow batching

Completed as a non-visual validation cut. `CDirectX12DrawPortBatch` captures
points, unpatterned lines, solid borders, and four-corner color rectangles
while their original D3D9 calls remain active. It converts DrawPort raster
coordinates to clip space, preserves each physical scissor rectangle, merges
consecutive primitives with the same scissor, and uploads one dynamic vertex
stream per in-flight frame.

`CDirectX12PipelineCache` now distinguishes textured validation and solid 2D
pipeline variants. The solid pipeline consumes position and vertex color,
uses the legacy source-alpha blend mode, and reuses cached PSOs keyed by
render-target format and multisample description.

The captured batch is replayed after the D3D9On12 render target is acquired,
with vertex alpha forced to zero. This exercises the real upload, PSO,
viewport, scissor, render-target, and `DrawInstanced` path without changing
the visible frame or incorrectly reordering individual DrawPort calls around
other D3D9 rendering. Visible cutover must happen at the boundary of a
coherent 2D pass, where its ordering relative to world rendering and UI
layers can be preserved explicitly.

### Stage 5c: complete solid DrawPort command coverage

Completed in shadow-validation mode. The solid command stream now accepts
arbitrary triangles in addition to points, expanded one-pixel lines, borders,
and rectangles. Opaque DrawPort clears and the legacy `AddTriangle` and
untextured `AddQuadrangle` queue paths are captured as well.

All solid commands share one append routine that preserves submission order,
coalesces only adjacent commands with the same scissor rectangle, and records
the exact triangle count. Textured quadrangles are deliberately excluded when
the legacy texture stage is active, preventing a colored approximation from
being mistaken for a valid migration.

The main UI boundary was identified in `CUIManager::Render`. It cannot switch
visibly while only solid commands are native because that pass interleaves
backgrounds, textured widgets, text, tooltips, models, and the cursor. Replaying
only its solids at frame end would place backgrounds over later UI content.
The next required stage is therefore an ordered textured-quad path, followed
by font batches, before the complete UI pass can disable its D3D9 submissions.

### Stage 5d: interoperable textured DrawPort shadow batching

Completed in shadow-validation mode. The four principal `AddTexture`
overloads now capture full textures, atlas regions, arbitrary UV mappings,
and per-vertex colors. Quad indices retain the legacy winding and each
triangle records the active D3D9 texture, physical scissor rectangle, and
DrawPort offset before the compatibility queue changes state.

`CDirectX12InteropTextureManager` unwraps each distinct default-pool D3D9
texture only once per frame, allocates a shader-visible SRV, transitions it
from `COMMON` to `PIXEL_SHADER_RESOURCE`, and returns it to D3D9On12 with the
native frame fence. Managed textures use persistent native mirrors instead,
because D3D9On12 cannot unwrap them safely.

`CDirectX12DrawPortTextureBatch` owns the dynamic position/UV/color stream and
coalesces only adjacent triangles that share texture and scissor state. The
new textured 2D PSO multiplies the sampled texel by vertex color and shares
the existing root signature and sampler table. Shadow replay still forces
vertex alpha to zero, so the legacy UI remains the visible source while the
real interop texture, SRV, PSO, draw, fence, and presentation lifecycle run.

The standalone backend probe now also creates and unwraps a real D3D9 texture,
creates its D3D12 SRV, executes the exact colored-texture input layout and
shader pair, transitions the texture back to `COMMON`, and returns both the
texture and render target through the same fence. The next prerequisite for a
visible UI cutover is native font batching plus explicit capture of sampler
and blend variants used by the UI.

### Stage 5e: native font capture and render-state variants

Completed in shadow-validation mode. Both legacy text paths are covered:
`PutText` captures the classic font quads before their immediate
`gfxFlushQuads`, while `EndTextEx` captures the indexed atlas batches used by
the Korean, Chinese, Japanese, Thai, Brazilian, Russian, and other localized
font paths. Capturing at the flush boundary avoids duplicating glyph-layout
logic and preserves bold, italic, shadow, per-character color, atlas page,
indices, DrawPort offset, and scissor data.

Text and ordinary textured UI geometry now share the same native textured
batch. Each adjacent range also records its sampler and predefined blend
mode. The native sampler table contains point and linear filtering combined
with clamp and repeat addressing. The PSO cache covers opaque, alpha blend,
shade, additive, additive-alpha, multiply, and inverse-multiply equations.
The legacy transparent mode uses a dedicated pixel shader with the original
`alpha >= 128/255` test and blending disabled.

Shadow replay remains non-visual: alpha-blended vertices are forced to zero,
and alpha-tested glyphs are discarded. The extended backend probe validates
all four sampler descriptors and the dedicated alpha-test PSO while sampling
an unwrapped D3D9 texture and returning it through the native frame fence.

The next stage must preserve ordering across solid and textured DrawPort
commands. They are currently captured independently and replayed as two
groups, which is safe only while transparent validation is active. A unified
ordered command stream is required before a coherent UI pass can become
visibly native.

### Stage 5f: unified ordered DrawPort command stream

Completed in shadow-validation mode. The separate solid and textured batches
were replaced by `CDirectX12DrawPortCommandBatch`. Every 2D primitive now uses
one position/UV/color vertex format and enters the same ordered range list.
Solid primitives bind the renderer's persistent white texture, so they share
the textured PSO path without duplicating geometry or requiring a second
replay pass.

Capture now follows legacy submission boundaries rather than construction
time. The common `AddTexture`, `AddTriangle`, and `AddQuadrangle` arrays are
recorded at `FlushRenderingQueue`; immediate `PutTexture`, lens-flare,
accumulation-blend, classic text, localized text, and button-array flushes are
recorded immediately before their corresponding D3D9 draw. This preserves
cases where queued geometry is interleaved with direct primitives before a
flush.

Ranges are coalesced only when adjacent commands share texture (or the solid
white fallback), scissor, sampler, and blend mode. Rendering walks those
ranges in capture order and switches PSO, descriptor tables, and scissor as
needed. The obsolete solid batch, solid shader, and solid PSO variant were
removed.

The stream is still replayed with zero effective alpha, so this stage does not
change the visible frame. The next safe step is to define a bounded UI-pass
capture scope and compare its complete native replay before disabling the
matching D3D9 submissions.

### Stage 5g: bounded UI capture and visible comparison

Completed with the D3D9 fallback still enabled. The main
`CUIManager::Render` call now runs inside a reusable
`CDirectX12DrawPortScope`. Each native command range records its capture
scope, and adjacent ranges are merged only when their scope also matches.
The RAII boundary restores the previous scope automatically and can be reused
for later nested passes.

Shadow validation remains the default. Hidden ranges now use the alpha-blend
PSO as well as zero vertex alpha, which prevents opaque legacy blend modes
from accidentally writing visible RGB while the stream is being validated.
Two opt-in comparison modes expose only the scoped UI commands:

- `LASTCHAOS_DX12_UI_COMPARE=overlay` replays the complete captured UI layer
  visibly over the legacy result.
- `LASTCHAOS_DX12_UI_COMPARE=split` restricts that replay to the right half
  of the render target.
- An unset variable, or any other value, selects non-visual shadow mode.

Set the environment variable before launching the client. These modes are
diagnostic overlays rather than final replacement: D3D9 remains underneath
and continues to render UI models, render targets, world-name projections,
and any DrawPort operation not yet captured. In particular, translucent
pixels can appear darker or brighter where both implementations overlap.

The next cutover step is to mark foreign D3D9 draws interleaved with the UI
stream and split native replay into matching ordered segments. Once those
boundaries are explicit, covered DrawPort submissions can be disabled inside
the UI scope without moving native widgets in front of embedded 3D models.

### Stage 5h: UI barriers and ordered segment metrics

Completed without disabling legacy rendering. Render-to-texture transitions
inside the UI pass now insert explicit begin and end barriers into the native
DrawPort stream. Instrumentation lives in `CRenderTexture::Begin` and
`CRenderTexture::End`, so Cash Shop previews, custom-title previews, animated
item models, and future UI render targets share the same boundary mechanism.
Calls outside the scoped UI pass are ignored.

Every command range records a monotonically increasing segment identifier.
A barrier prevents ranges on opposite sides of a foreign D3D9 render from
being merged even if their texture, scissor, sampler, and blend state match.
The barrier kind and resulting segment are retained for the later
interoperability scheduler.

The native renderer exposes UI primitive, non-empty segment, and barrier
counts. When `LASTCHAOS_DX12_UI_COMPARE` selects `overlay` or `split`, the
backend reports these values only when they change:

```text
DX12 UI: 1842 primitivas, 2 segmentos, 2 barreras D3D9.
```

The visible comparison still replays all segments at frame end and D3D9
remains authoritative. Segment metadata alone does not yet make replacement
safe. The next stage must schedule each completed native segment at its
recorded D3D9On12 queue boundary, with explicit render-target ownership and
fence ordering, before selectively suppressing its equivalent DrawPort calls.

### Stage 5i: diagnostic in-order segment submission

Completed for the opt-in `overlay` and `split` comparison modes. When a UI
render target begins, the backend now submits every completed native UI
segment before D3D9 changes render targets:

1. obtain the current D3D9 render target;
2. end the D3D9 scene, which places preceding legacy work on the shared queue;
3. unwrap and transition the main target and the textures used by the segment;
4. record and execute only the pending segment range;
5. return all underlying resources with a native fence;
6. wait for that diagnostic submission, recycle its descriptors, reset the
   command allocator, and begin a new D3D9 scene.

The final frame replay starts at the first segment not already submitted, so
no native range is drawn twice. Shadow mode deliberately keeps the previous
single end-of-frame submission and therefore adds no mid-frame waits.

The synchronous fence wait makes allocator, dynamic vertex-buffer, and
descriptor reuse unambiguous while this path is being validated. It is not
the intended production scheduler. D3D9 rendering is still retained in all
modes, so a failure in the diagnostic submission reports an error while the
legacy UI remains visible.

The standalone probe now performs a second `BeginScene`/`EndScene`,
unwrap/transition/execute/return cycle on the same D3D9On12 backbuffer after
the first fence completes. This specifically validates the repeated
ownership transfer required by mid-frame segment submission.

The next stage should replace the diagnostic wait with per-submission command
allocators and transient vertex/descriptor generations. Once multiple native
segments can remain in flight safely, replacement mode can suppress covered
D3D9 DrawPort calls segment by segment.

### Stage 5j: asynchronous per-submission generations

Completed for diagnostic segmented replay. Mid-frame submissions no longer
wait on the CPU. Each of the three frame slots owns up to sixteen command
allocators. After executing a segment, the shared graphics command list is
reset with the next allocator while the previous allocator remains in flight.
The final fence stored for the frame slot covers every earlier submission on
the same queue.

Resources whose descriptors or contents may change also have independent
generations:

- the RTV heap contains one descriptor per frame and submission;
- every command-batch replay creates an immutable vertex-buffer generation;
- every newly unwrapped D3D9 texture receives a distinct SRV generation;
- returned texture resources and descriptors remain retained until the whole
  frame slot is recycled after its final fence.

`ReturnUnderlyingResource` carries each partial fence back into D3D9On12, so
legacy rendering can resume immediately and the shared queue supplies the
required GPU ordering. If a frame reaches the sixteen-submission capacity,
the scheduler leaves additional ranges pending for the normal final replay
instead of reusing an in-flight allocator or descriptor.

The standalone probe now performs its second backbuffer ownership cycle with
a different allocator and without waiting for the first native fence on the
CPU. A D3D9 clear between both cycles validates that D3D9On12 observes the
returned fence before the resource is unwrapped again. The same asynchronous
cycle also unwraps and returns the D3D9 texture a second time while retaining
the first native resource generation.

The next stage can introduce an explicit replacement mode that suppresses
only successfully captured DrawPort submissions inside the UI scope. Shadow,
overlay, and split modes must remain available as independent fallbacks.

### Stage 5k: opt-in replacement with per-command fallback

Completed as an explicit diagnostic cutover. Set
`LASTCHAOS_DX12_UI_COMPARE=replace` before launching the client to make the
native DX12 DrawPort stream authoritative inside `CUIManager::Render`.
`shadow`, `overlay`, and `split` retain their previous behavior, and an unset
variable still selects shadow validation.

Replacement is decided at each legacy submission boundary. A D3D9 DrawPort
draw is omitted only when all of the following are true:

- replacement mode is active;
- a DX12 frame and the bounded UI capture scope are active;
- the complete primitive, quad group, or indexed element group was accepted
  by the native command stream;
- a required D3D9 texture can be unwrapped for native use.

If any condition fails, the original D3D9 submission is preserved. This
applies to points, solid lines and borders, color clears and rectangles,
classic and international text, button batches, queued elements,
`PutTexture`, lens flares, and `BlendScreen`. Patterned lines and any
uncaptured operation intentionally remain on the legacy path.

The backend reports replacement coverage when its counters change:

```text
DX12 UI replace: 87 envios D3D9 omitidos, 2 fallbacks por captura.
```

This stage does not remove the D3D9 device. World and model rendering, UI
render-to-texture content, 3D DrawPort primitives, and other uncaptured
graphics paths still execute through D3D9On12. The replacement mode is
therefore opt-in until representative screens have been compared for
geometry, clipping, filtering, blending, text, render-target transitions,
and device reset behavior.

The next stage should run that visual coverage pass, classify every fallback,
and migrate any missing high-frequency UI operation. Once replacement is
visually equivalent and fallback-free on the target screens, it can become
the default UI path before beginning the separate 3D renderer conversion.

#### Runtime validation findings

The first full-client replacement run exposed two integration issues that the
standalone probe could not reproduce:

- the installed legacy `D3DCOMPILER_43` rejects Shader Model 5.1 profiles, so
  the native shaders now use the D3D12-compatible `vs_5_0` and `ps_5_0`
  profiles and report detailed compilation errors;
- retaining the engine's indirect current-texture pointer could reference a
  destroyed object. Capture now obtains the actually bound texture through
  `IDirect3DDevice9::GetTexture` and owns its COM reference explicitly.

Most client textures are created in `D3DPOOL_MANAGED`. D3D9On12 returns
`E_INVALIDARG` when `UnwrapUnderlyingResource` is requested for these
textures. Capture therefore rejects them before enqueueing and preserves the
matching D3D9 submission. A validated run reached `Loading Session`, remained
active, and reported zero DX12 submission errors with 15–16 safe texture
fallbacks per frame.

### Stage 5l: managed texture mirrors and native DXGI presentation

Completed. Managed D3D9 textures that cannot be unwrapped are mirrored into
native DEFAULT-heap D3D12 textures. Every mip is copied through the reusable
upload manager, format conversion and component mapping are preserved, and
the mirror is cached for the lifetime of the attached D3D9 device. Recreating
these mirrors every in-flight frame caused repeated full-atlas uploads and
eventually a GPU watchdog reset; persistent mirrors remove that pressure.

The mirror path also expands `A4R4G4B4` and `X4R4G4B4` to BGRA8. A failed
texture acquisition now skips only the affected native range rather than
aborting the rest of the UI command list, although the validated replacement
run currently reports zero skipped ranges.

The replacement path also owns presentation. The shared D3D9On12 backbuffer
is completed by the native UI pass, copied on the D3D12 queue into a
double-buffered `IDXGISwapChain3`, and presented through DXGI. The D3D9
`Present` call is not used in replacement mode.

### Stage 5m: complete end-of-frame 2D overlay scope

Completed. The native UI scope now remains active after
`CUIManager::Render` and closes immediately before the frame's `EndScene`.
Cursor and console DrawPort submissions therefore enter the same ordered
native stream as the widgets and fonts instead of escaping through the
default legacy scope.

The backend owns the close operation and drains any nested UI depth before
presentation, so error paths cannot leave a capture scope open across frame
reuse.

An in-world validation at 1600x900 initially exposed an incomplete HUD:
one unsupported managed `A4R4G4B4` texture aborted the replay loop, leaving
only the ranges submitted before it. After adding format conversion,
range-local failure handling, and the persistent managed-texture cache, the
same `replace` path rendered the player panel, target panel, complete
minimap, chat, action bar, quest panel, and modal dialogs. The session
remained responsive during active gameplay and produced no new NVIDIA
watchdog or application-hang event during the validation interval.

The next migration boundary is the 3D renderer. A first bounded cut should
capture one static world/model geometry path with its transform constants,
depth state, texture bindings, and indexed draw into a native D3D12 PSO
before suppressing the matching D3D9 submission.

### Stage 6a: bounded dynamic indexed 3D shadow path

Completed. The central dynamic `DrawElements_D3D` route now feeds a dedicated
`CDirectX12Legacy3DCommandBatch`. The capture boundary is intentionally
restricted to fixed-function triangle lists with CPU positions, UV0, one
texture pass, and no projective mapping or programmable vertex/pixel shader.
Unsupported submissions are counted and continue exclusively through D3D9.

For each accepted submission the batch owns copies of the vertices and
indices, obtains the active world/view/projection matrices and viewport, and
retains the bound D3D9 texture through a COM reference. Positions are
transformed into clip space on capture. At frame submission, concatenated
32-bit index and vertex buffers are uploaded once and replayed with the
native validation PSO, native sampler, and the existing managed/unwrapped
texture bridge.

This first replay is a shadow pass: its pixel shader emits zero alpha and
depth is disabled, so it exercises the complete native resource and indexed
draw path without changing the authoritative frame. D3D9 is not suppressed
for 3D yet. The backend reports coverage when counts change and can also
append the same telemetry to the path selected by
`LASTCHAOS_DX12_VALIDATION_LOG`.

A full-client validation at 1600x900 captured both startup and login
geometry. Representative frames reported:

```text
DX12 3D sombra: 5 envios capturados, 0 rechazados, 10 triangulos.
DX12 3D sombra: 2 envios capturados, 19 rechazados, 4 triangulos.
```

The client remained active in UI `replace` mode and visual comparison
confirmed that the login composition was unchanged. The next cut should
classify the nineteen rejected submissions, add native depth resources and
PSO state mapping, and select one high-coverage opaque 3D family for visible
overlay comparison before any legacy 3D draw is suppressed.

### Stage 6b: native depth state and opt-in 3D overlay

Completed. The bounded legacy 3D batch now owns a reusable
`CDirectX12DepthBuffer`. It creates a `D32_FLOAT` resource and DSV compatible
with the active render target dimensions and MSAA sample description, clears
it once before native 3D replay, and recreates it only when compatibility
changes.

Each captured indexed range records the D3D9 depth-enable, depth-write,
comparison function, cull mode, alpha-test, and blending state. The pipeline
cache includes those depth/cull values in its PSO key and provides separate
native 3D shadow and overlay variants. Vertices now preserve either the
active color array or the legacy constant color in addition to position and
UV0.

Shadow remains the default. Setting
`LASTCHAOS_DX12_3D_COMPARE=overlay` enables visible replay for accepted
opaque ranges only; blended and alpha-tested ranges remain shadow-only until
their complete blend/alpha state is mapped. The overlay still retains the
matching D3D9 draw and is therefore a comparison mode, not replacement.

Rejected submissions are now classified. Validation on the login screen
reported:

```text
DX12 3D overlay: 2 envios capturados, 19 rechazados, 4 triangulos;
motivos dinamico=19, VS=0, PS=0, proyecto=0, pasadas=0, arrays=0,
limite=0, indice=0, estado=0.
```

This proves that the next coverage blocker is the static vertex-buffer path,
not programmable shaders or multipass texturing on this screen. Visual
validation at 1600x900 found no difference in the login composition while
overlay mode was active. The next stage should mirror or expose static
position/UV/color streams when `d3d_SetSubBuffer` selects them, then feed
those nineteen submissions into the same native command batch.

### Stage 6c: static vertex-buffer CPU mirrors

Completed as capture infrastructure. Legacy static vertex buffers now
maintain a CPU-visible mirror that can be consumed by the native D3D12
command batch. Read/write buffers
share their existing read array; write-only and dynamic buffers allocate an
owned mirror, avoiding duplicate storage where the engine already retains a
CPU copy. Mirror ownership and failure cleanup remain encapsulated in
`VertexBuffer`.

Static lock/unlock records the exact written range and copies it into the
mirror before the D3D9 buffer is unlocked. Position, UV0, and D3D color
sub-buffer selections forward their mirror pointer, vertex range, and stride
through the backend. The reusable legacy 3D batch accepts either dynamic
arrays or those static sources; legacy D3D ARGB colors are converted back to
the engine RGBA layout at this boundary.

The first full in-world shadow replay was not stable: Windows recorded
`LiveKernelEvent 141` GPU watchdog resets while the redundant native 3D pass
was active. Native 3D capture is therefore disabled by default and requires
the explicit opt-in `LASTCHAOS_DX12_3D_CAPTURE=enabled`. Static CPU mirrors
remain available, but they are not replayed during the stable UI replacement
configuration. Representative pre-gate telemetry reported:

```text
DX12 3D sombra: 7 envios capturados, 329 rechazados, 243 triangulos;
motivos streams=0, VS=320, PS=0, proyecto=0, pasadas=9,
arrays=0, limite=0, indice=0, estado=0.
```

The zero `streams` count confirms that static position/UV selection is no
longer the coverage blocker. Most remaining world submissions use a
programmable vertex shader, and nine are multipass. Those submissions still
run only through D3D9On12; no unsupported legacy draw is suppressed.

The next 3D step must first replace the unsafe redundant shadow replay with a
bounded native path that has explicit resource ownership, workload limits,
and device-removal recovery. Only then should it classify the active legacy
vertex-program families and migrate one high-coverage family, including its
input layout, constants, and shader behavior, into a native D3D12 PSO.

### Stage 6d: bounded native 3D probe and device-loss diagnostics

Completed and validated. Native 3D capture now has two explicit profiles.
The diagnostic `probe` profile accepts at most one compatible draw every 120
frames, with hard limits of 4096 vertices and 12288 indices. The continuous
profile remains opt-in and is capped at eight draws, 32768 vertices, and
98304 indices per frame. Unsupported draws remain authoritative on D3D9On12
and are never suppressed.

Fence waits now time out after ten seconds instead of waiting forever.
Command-list close, queue signal, fence-event, timeout, and device-removal
failures report both the operation result and
`ID3D12Device::GetDeviceRemovedReason`. This makes a GPU reset diagnosable
without leaving the client permanently blocked.

The `LCRelease|x64` build was exercised with UI `replace` for 60 seconds and
with `LASTCHAOS_DX12_3D_CAPTURE=probe` for 180 seconds. The probe repeatedly
replayed one compatible submission containing two triangles while the client
remained responsive and memory stayed stable. Windows recorded no NVIDIA,
Display, application-hang, or application-error events. Login, server
selection, character selection, the world scene, and the complete HUD were
also verified visually at 1600x900.

This stage makes native capture safe enough for classification and targeted
experiments; it does not yet replace the authoritative D3D9On12 world pass.
The next implementation step is to fingerprint the rejected programmable
vertex-shader families and migrate the highest-coverage opaque family into a
dedicated native D3D12 PSO.

### Stage 6e: shader-family fingerprinting and rigid-lit native PSO

Completed as a bounded, opt-in migration slice. Vertex shader bytecode and
the active vertex declaration are now fingerprinted with stable 64-bit FNV-1a
hashes. Per-sample telemetry reports draw and triangle coverage for the most
active family, and an optional diagnostic directory stores unique vertex and
pixel bytecode plus disassembly without adding those artifacts to source
control.

The dominant measured vertex family is `88CDE6E1231B48B2`: 78 draws and
11698 triangles in the classification sample. Its associated pixel shader is
`4E91DDD261F074A2`. The original pair uses position, normal, and base UV
inputs; 13 vertex constant registers; two pixel constant registers; a base
texture; and a scaled detail texture.

A dedicated rigid-lit DX12 pipeline now reproduces that exact pair:

- row-wise world/view/projection transforms from `c0` through `c3`;
- normalized directional lighting with the original `c7.x/c7.y` clamp;
- diffuse and ambient terms from `c5` and `c6`;
- independent `TEXCOORD0` and scaled `TEXCOORD1` outputs;
- base/detail texture multiplication with the two pixel constants;
- separate shadow and visible overlay PSO variants.

Dynamic and static normal streams are mirrored into the native 48-byte vertex
layout. The root signature exposes both SRV tables and both constant blocks.
Only the exact supported VS/PS pair is accepted; every other programmable
submission remains authoritative on D3D9On12.

The shadow test captured 87 triangles with zero vertex- or pixel-shader
rejections. The final visible overlay test captured one 20-triangle draw per
sample. Login, server selection, character selection, the 3D world, minimap,
chat, and the complete HUD were verified at 1600x900. Two visual states five
seconds apart remained coherent, and an additional 30-second isolated run
passed with a responsive process and no NVIDIA, Display, device-removal,
application-hang, or application-error events.

The validated engine SHA-256 is
`D69601624280EACF34CD70DDA8A5F7FFD6ED907FBBC5E4D74306C093F77FAB4B`;
the report is `.itconfig/validation-20260724-155000.json`. This stage proves
the highest-coverage rigid-lit pair can execute natively, but it intentionally
does not yet suppress the legacy authoritative draw. The next step is an
automated backbuffer comparison and only then replacement of this exact pair.

### Stage 6f: authoritative rigid-lit replacement with shared depth

Completed and validated behind the explicit rollout profile. The replacement
does not use the private diagnostic depth buffer: the current D3D9On12
depth/stencil surface is unwrapped together with the color target, transitioned
to `DEPTH_WRITE`, bound through a matching DSV/PSO format, and returned to D3D9
with the same fence. This preserves occlusion against every legacy family that
has not migrated yet.

Suppression is fail-safe and frame-consistent. The first frame verifies depth
interop without omitting legacy work. From the next frame onward, a D3D9 draw
is skipped only when the replacement profile is enabled, the exact validated
VS/PS pair was captured, and shared depth succeeded in the previous frame.
Static position, normal, UV, and color mirrors are accepted for this family.
Unsupported shaders, multipass draws, missing arrays, invalid indices, or
unavailable depth always fall back to D3D9On12.

The reusable validation profile is:

```powershell
powershell -ExecutionPolicy Bypass `
  -File .itconfig/Invoke-LastChaosValidation.ps1 `
  -DurationSeconds 180 `
  -UiMode replace `
  -Native3DRigidLitReplace
```

An automated visual comparator now supports aligned pixel comparison and a
camera-independent central-world histogram/luminance check. The final A/B
sample passed with histogram distance `10.1545` and luminance delta `8.4194`.
In active world frames the client captured and replaced up to 58 rigid-lit
draws; a representative steady frame omitted 44 D3D9 draws while leaving 353
unsupported submissions on the legacy fallback.

The 180-second run and an additional isolated 30-second window both passed.
The final window contained 15 responsive samples, no Display/NVIDIA events,
and no application hang/error events. The verified engine SHA-256 is
`BFC71CE02ABF862A345C6AE8C65F2FDEE6C92D769E1BC12C346D79C776BE452B`.
Evidence is stored outside Git in `.itconfig/validation-20260724-162232.json`,
`.itconfig/dx12-rigid-lit-comparison.json`, and the associated A/B captures.

This removes D3D9 authority for the validated rigid-lit family when the rollout
profile is active. Other shader families and multipass geometry remain the next
3D migration targets.

### Etapa 6g: familias heredadas y geometría multipass

Completada. El inventario de parejas activas se convirtió en descriptores
reutilizables de familias VS/PS. La ruta genérica DX12 cubre skinning de hasta
cuatro huesos, iluminación, coordenadas proyectadas, reflexión, normal mapping,
color de vértice y terreno de cuatro capas. Los streams CPU incluyen posición,
normal, cuatro UV, tangente, índices/pesos, dos colores y `clipW`.

La firma raíz expone cuatro SRV y dos bloques de constantes sin superar el
límite de 64 DWORD. Los PSO conservan profundidad compartida, culling, alpha
test y las mezclas opaca, alpha, aditiva y multiplicativa. El terreno
`CB70C2B162AECF3F/10848222350BDA01` genera sus cuatro UV desde posición; por
eso no exige UV de origen y puede reemplazar sus 32 lotes estáticos.

La reproducción 3D se envía al comenzar el alcance de UI. Esto mantiene el
orden mundo -> HUD y evita que un terreno nativo compuesto al final del frame
cubra la interfaz. El modo inventario no abre una pasada visible, y el
reemplazo completo sólo se activa cuando existe el archivo de compuerta.

Se validaron por separado:

- terreno proyectado de cuatro texturas y su mezcla multipass;
- skinning iluminado `03F0F9B6ED714154/C266DC4B1D39418F`, con 10 envíos y
  2211 triángulos por muestra;
- reemplazo integral de todas las familias conocidas, con 347 envíos y
  aproximadamente 146600 triángulos por frame.

La prueba integral conservó personaje, edificios, terreno, árboles, HUD,
chat, minimapa y barra de acciones sin deformaciones ni bloqueos. Los rechazos
restantes no son familias desconocidas: corresponden a UI/profundidad
desactivada, índices inválidos o dos arrays ausentes y permanecen en fallback
seguro. El binario validado tiene SHA-256
`FCDC8B035492E2995ECA65E89965B31CB09D963431340AFDB7DF047044572B5B`.

### Etapa 6h: estabilización del reemplazo por familias

La validación prolongada de todas las familias genéricas reveló que el conteo
de envíos aceptados no era suficiente para declarar equivalencia visual. Al
reemplazarlas simultáneamente aparecieron polígonos gigantes y geometría
deformada. El reemplazo completo ahora aplica una compuerta conservadora:
sin un selector explícito sólo suprime D3D9 para la familia rígida ya
validada. Las demás familias continúan disponibles para inventario y pruebas
aisladas hasta aprobar su equivalencia visual.

También se corrigió una regresión de orden entre 3D y UI. El envío temprano de
geometría DX12 consultaba el segmento cero antes de que el HUD hubiese grabado
comandos y avanzaba el cursor de segmentos aunque todavía no hubiera UI. Al
final del cuadro, el HUD recién grabado quedaba detrás del cursor y no se
reproducía. Ahora el cursor sólo avanza cuando el rango contiene comandos de
UI enviados correctamente.

Validación del arreglo:

- mundo, personaje, edificios y terreno sin deformaciones;
- HUD, chat, minimapa y barra de acciones completos;
- 41 envíos 3D autoritativos de D3D9 omitidos en una muestra;
- entre 78 y 83 envíos de UI de D3D9 omitidos por cuadro;
- proceso responsivo después de 129 segundos;
- sin `DXGI_ERROR_DEVICE_REMOVED`, `DXGI_ERROR_DEVICE_HUNG` ni errores DX12;
- `Engine.dll` SHA-256
  `F808F3D1277024E994E765C4131D7858A2DFEB2A005602E51594CDBFA2DA47CF`;
- informe `.itconfig/validation-20260724-193738.json`.

Durante una ejecución posterior a una reconstrucción completa, la creación del
display falló con menos de 800 MB de memoria física libre y el cliente entró en
su recuperación OpenGL heredada, donde terminó con una violación de acceso.
No fue una pérdida de dispositivo DX12. Al finalizar el servidor de símbolos
de la compilación y recuperar memoria, la misma DLL creó correctamente el
contexto Direct3D. Las validaciones deben distinguir este fallo de inicialización
por presión de memoria de un TDR en ejecución.

### Etapa 6i: segunda familia rígida, arranque y culling

La familia VS `F1D814903AF5DCC7` se validó de forma aislada y luego se agregó
al conjunto estable. En la prueba integrada, las dos familias autorizadas
reemplazaron entre 77 y 169 envíos por cuadro según la vista, sin rechazos de
captura para los lotes reproducidos. Las familias restantes continúan en
fallback seguro.

Se corrigieron tres defectos descubiertos durante la validación prolongada:

- un envío 3D anticipado ya no consume un segmento de UI todavía vacío;
- el primer cuadro DX12 no se presenta hasta capturar comandos de UI, evitando
  mostrar pies o geometría parcial durante el arranque;
- la conversión de `D3DCULL_CW` y `D3DCULL_CCW` respeta que los PSO usan
  `FrontCounterClockwise = FALSE`. La traducción anterior descartaba el lado
  opuesto y hacía desaparecer fachadas de edificios.

También se ordenó el apagado para destruir el backend DX12 antes del dispositivo
D3D9On12 y se agregó invalidación explícita de texturas heredadas. Esto elimina
las referencias obsoletas encontradas por los dumps de cierre.

Validación final:

- login, servidor, personaje, mundo y HUD verificados visualmente;
- torres, fachadas, arcos, pisos y paredes completas desde varios ángulos;
- 61 muestras durante 129 segundos, 56 responsivas; las cinco no responsivas
  corresponden a la carga síncrona inicial;
- cierre solicitado por ventana y terminado sin excepción;
- cero eventos de aplicación, pantalla o retirada de dispositivo;
- sin nuevos dumps de `Nksp.exe`;
- informe `.itconfig/validation-20260724-200600.json`;
- `Engine.dll` SHA-256
  `862D15D1DF11A23F136B25D33AF2991038C69549158AC7CD3AC1197C81CB523C`.

### Etapa 6j: migración completa de las familias 3D programables

Las 14 familias semánticas de vertex shader catalogadas ya tienen traducción
DX12 autoritativa. El catálogo contiene 18 huellas VS y 18 huellas PS porque
algunas familias tienen más de una variante binaria. La autorización se hace
mediante una tabla de 22 parejas VS/PS exactas; una combinación no inventariada
no puede reutilizar accidentalmente un pixel shader incompatible.

La cobertura incluye:

- geometría rígida iluminada, proyectada y con reflexión;
- skinning de posición, iluminación y espacio tangente;
- normal mapping, detalle y reflexión combinados;
- color de vértice;
- terreno proyectado de una, dos y cuatro capas;
- alpha-test y mezclas opaca, alpha, aditiva y multiplicativa;
- pixel shaders de constante, máscara alpha y selección por alpha.

Se corrigió la traducción de skinning al reproducir el orden real del bytecode
D3D9: los índices se leen como `v3.zyxw`, los pesos explícitos como
`v2.zyx`, y el cuarto peso es el remanente. La lectura anterior en orden
`xyzw` explicaba los polígonos gigantes observados al activar todas las
familias simultáneamente.

Las variantes nuevas de material se implementaron como modos reutilizables del
shader genérico: base-alpha con reflexión, base-alpha con detalle y normal map
con reflexión. También se agregaron los contratos rígidos de tangente,
tangente proyectada e iluminación reflejada.

Validación:

- selección de personaje con skinning posicional, partículas y transparencias;
- mapa de Orythia con personaje, edificios completos, terreno multipass,
  árboles, minimapa, chat y HUD;
- 46 muestras durante 99 segundos, 42 responsivas; las cuatro muestras no
  responsivas pertenecen a la carga síncrona inicial;
- cierre limpio, sin eventos de aplicación, pantalla o retirada de dispositivo;
- informe aprobado `.itconfig/validation-20260724-202534.json`;
- capturas `.itconfig/dx12-skinned-position-families-20260724.jpg` y
  `.itconfig/dx12-all-shader-families-final-20260724.jpg`;
- `Engine.dll` SHA-256
  `E7A3AE2735730310B8D9BA227926564A5023A76FADBDD3FB8C4820646F0470FE`.

Los envíos 3D de función fija `VS=0/PS=0` no son una familia de shaders y
permanecen como el siguiente frente de eliminación del fallback D3D9On12.

### Etapa 6k: automatización de login y diagnóstico fixed-function

El cliente acepta argumentos de prueba para recorrer sin intervención el
login, la selección de servidor y la entrada al mundo:

```text
Nksp.exe +testautologin <usuario> <contraseña> +testserver 0 \
  +testchannel 0 +testcharacter 0
```

La lógica está aislada en `Engine/Testing/ClientTestAutomation` y se ejecuta
como una máquina de estados reutilizable. Espera cada etapa real del cliente,
selecciona índices válidos y borra su copia de la contraseña después de enviar
el login. El parser no registra la línea de comandos completa para impedir que
la contraseña aparezca en `Nksp.log`. El harness
`.itconfig/Invoke-LastChaosValidation.ps1 -AutoLogin` toma las credenciales del
archivo de configuración local excluido de Git y permite elegir servidor,
canal y personaje.

Durante la traducción de `VS=0/PS=0` se implementaron:

- interpretación reutilizable de `D3DTOP`, `D3DTA`, `RESULTARG` y constante
  por etapa;
- selección y transformación de coordenadas mediante
  `TEXCOORDINDEX`/`TEXTURETRANSFORMFLAGS`;
- samplers point, linear y anisotrópicos con clamp o repeat;
- clasificación explícita del backbuffer frente a render targets auxiliares;
- diagnósticos de viewport, volumen clip, fuente de vértices y estado
  multipass.

La prueba visual encontró dos contratos todavía no equivalentes: capas fixed
transparentes sin escritura de profundidad y geometría que cruza el plano
homogéneo de cámara. También se aisló un pase opaco de textura 2048×2048 que
puede cubrir el mundo. Por seguridad, `VS=0/PS=0` no se autoriza aún como
reemplazo estable; las 22 parejas programables continúan autoritativas en
DX12. Esto evita reintroducir los paneles gigantes mientras se completa el
pipeline fixed y su composición multipass.

Validación de la automatización:

- progreso autónomo por etapas `LOGIN`, `SELSERVER`, `SELCHAR`, carga y juego;
- entrada al mundo con la cuenta de pruebas y proceso responsivo;
- cierre limpio e informe aprobado
  `.itconfig/validation-20260724-213356.json`;
- `Engine.dll` SHA-256
  `AF58B03621FC3DA715C2E0097EF60F267D05BCF4E298B6302A1A44FEB2FAD927`.

### Etapa 6l: validación autoritativa y orden multipass

La prueba automática combinada demostró que “implementado” y “autorizado para
reemplazo” deben ser estados distintos. Las 22 parejas VS/PS continúan
catalogadas e implementadas, pero sólo estas dos omiten actualmente su draw
D3D9:

- `88CDE6E1231B48B2 / 4E91DDD261F074A2`;
- `F1D814903AF5DCC7 / C266DC4B1D39418F`.

Al autorizar simultáneamente todas las parejas y `VS=0/PS=0`, el batch DX12 se
reproducía al final del cuadro y alteraba el orden relativo con las capas que
seguían componiéndose mediante D3D9On12. Los síntomas eran paneles
intermitentes, suelo uniforme o negro y objetos aparentemente flotantes.

Inicialmente se añadió un flujo de eventos de limpieza de profundidad,
incluyendo rectángulos parciales. La etapa 6n determinó que reproducir esos
eventos era incorrecto porque el `Clear` ya había ocurrido en el depth
compartido de D3D9On12. Fueron sustituidos por barreras previas al borrado.
Hasta completar las pasadas fixed transparentes y multipass, la compuerta de
reemplazo sólo promueve parejas que superaron una prueba visual combinada.

Validación:

- login automático, servidor, personaje y entrada al mundo;
- 111-119 envíos 3D DX12 por cuadro, 19 410-21 959 triángulos;
- piso texturado, edificios completos, vegetación, estatua, HUD y minimapa;
- 108 segundos de ejecución, cierre limpio y cero eventos de aplicación,
  pantalla o dispositivo;
- informe `.itconfig/validation-20260724-220528.json`;
- captura `.itconfig/dx12-stable-families-world-20260724.jpg`;
- `Engine.dll` SHA-256
  `3CB47D58431B8114F7EFCCB75E14330D7E3D45E6C2EB678CD27D0542D0B196F4`.

### Etapa 6m: cobertura completa del inventario y composición ordenada

El inventario automático encontró tres parejas adicionales durante recorridos
más amplios de Orythia. Se implementaron sin promoverlas todavía:

- `3217ECE2D2C1D96A / 5B3BD26F0B904B3D`;
- `56FBA5FFC803EDB0 / D9A8DB50746FD55D`;
- `7873727C8ED9D187 / 77162620F6305229`.

Con ellas, las 25 parejas VS/PS observadas están catalogadas e implementadas.
El conjunto autoritativo continúa limitado a las dos parejas que superaron la
validación visual combinada.

La reproducción 3D dejó de ser un único lote al final del cuadro. El batch
mantiene el índice del próximo rango pendiente y, antes de cada draw que debe
continuar por D3D9On12, envía solamente los rangos DX12 anteriores. Los
eventos de limpieza de profundidad conservan su posición absoluta dentro del
lote. Esto mantiene el orden original entre edificios, personajes, terreno
multipass, transparencias y efectos sin volver a dibujar rangos ya enviados.

Validación:

- desplazamiento por la plaza de Orythia con personaje delante y detrás de
  edificios, pedestales, árboles y estatuas;
- suelo texturizado y fachadas completas, sin estructuras superpuestas al
  personaje;
- 46 muestras, cierre limpio y cero eventos de aplicación, pantalla o
  dispositivo;
- 49-224 envíos DX12 por cuadro y hasta 26 758 triángulos durante el recorrido;
- informe `.itconfig/validation-20260724-222603.json`;
- captura `.itconfig/dx12-ordered-composition-20260724.jpg`;
- `Engine.dll` SHA-256
  `88932B6CFC2F99DFBBEBE8012235ADE9166639BE4DFF6BABEADF4D6DF17A1E97`.

El siguiente frente sigue siendo autorizar las otras 23 parejas una por una y
migrar `VS=0/PS=0`. La infraestructura de composición ordenada ya permite
hacerlo sin mezclar arbitrariamente el orden de ambos command streams.

### Etapa 6n: profundidad compartida y agua de las fuentes

La validación visual detectó que el agua de las fuentes podía aparecer como
franjas verticales o una lámina extendida sobre el piso. El problema sólo se
reproducía con las dos familias autorizadas simultáneamente; cada familia
aislada y la referencia D3D9On12 se veían correctamente.

La diferencia era la captura de `ClearDepth`. D3D9 ejecutaba el borrado sobre
el recurso de profundidad compartido y DX12 volvía a reproducir el mismo
evento al enviar el lote. Esa segunda limpieza histórica eliminaba
profundidad más reciente del terreno y permitía que el agua atravesara la
escena.

Se retiró por completo el almacenamiento de eventos `DepthClear`. Antes de
cada `IDirect3DDevice9::Clear`, el backend materializa únicamente los rangos
DX12 pendientes y devuelve el depth a D3D9On12; el borrado real se ejecuta una
sola vez. El mismo límite se usa para borrados completos y rectangulares.

Validación:

- comparación de referencia sin reemplazo 3D;
- pruebas aisladas de `88CDE6E1231B48B2` y `F1D814903AF5DCC7`;
- prueba combinada con recorrido por la plaza, suelo seco y estructuras
  correctamente ocluidas;
- 25 muestras, cierre limpio y cero eventos de aplicación, pantalla o
  dispositivo;
- informe `.itconfig/validation-20260724-224228.json`;
- capturas `.itconfig/dx12-water-depth-fixed-20260724.jpg` y
  `.itconfig/dx12-water-depth-fixed-walk-20260724.jpg`;
- `Engine.dll` SHA-256
  `E782D845B6D2B7440D626265856DEA335319FFC70CF260CCD986F29BC0304352`.

### Etapa 6o: promoción selectiva y terreno multipass estable

La compuerta de laboratorio por huella de vertex shader ahora adquiere
autoridad desde el primer draw capturado. El umbral de volumen de mundo se
mantiene para el reemplazo normal sin selector, pero ya no deja una familia
selectiva parcialmente duplicada entre D3D9On12 y DX12.

El terreno de cuatro capas reveló una diferencia de estado, no de geometría:
el renderer heredado usa `ONE / SRC_ALPHA`, mientras el traductor lo
convertía al blend alfa convencional. Se agregó el modo
`DX12_BLEND_TERRAIN_LAYER`, cuya PSO usa exactamente
`D3D12_BLEND_ONE / D3D12_BLEND_SRC_ALPHA`. Con esto el suelo conserva todas
sus capas al cambiar el ángulo de cámara.

También se tradujo `DEST_COLOR / SRC_COLOR` al modo multiplicativo usado por
las sombras proyectadas. Las siguientes parejas superaron pruebas selectivas
y fueron incorporadas al conjunto autoritativo:

- `6518E1E655486C62 / F769B9454292AD44`;
- `6518E1E655486C62 / E76E3479530FF2CB`;
- `BFDAAD52F7C28AAF / 000D90AFD69D7DA9`;
- `BFDAAD52F7C28AAF / 8BF0F79F73B2CD3C`;
- `03F0F9B6ED714154 / C266DC4B1D39418F`;
- `CB70C2B162AECF3F / 10848222350BDA01`;
- `20771BEF807EB60E / B3762CA90A8E2CCE`;
- `CF210A6C5DC33E8C / 391B8FA0541C9735`;
- `CF210A6C5DC33E8C / 7BD479383778B972`;
- `0BDAEBAB2645C412 / B5BD45A8BA08F65B`;
- `56FBA5FFC803EDB0 / 4BDD3F424E9CB4D1`;
- `56FBA5FFC803EDB0 / D9A8DB50746FD55D`;
- `58BF46CA623CB0F3 / 92778B02E5A59285`.

Sumadas a las dos parejas iniciales, hay 15 parejas programables autorizadas.
La regresión combinada procesó entre 223 y 285 draws DX12 por frame y llegó a
162 269 triángulos durante el recorrido. No hubo rechazos, errores de
dispositivo ni defectos de suelo, edificios, personaje o UI. El informe
automatizado conserva `Passed=false` únicamente porque el recorrido manual
continuó con el cliente abierto después de vencer su ventana de 100 segundos.

El selector de laboratorio también acepta
`LASTCHAOS_DX12_3D_REPLACE_PIXEL_FAMILY`. Puede combinarse con el selector VS
para ejercer una pareja exacta y evitar que dos pixel shaders que comparten
vertex shader se validen accidentalmente como un solo caso.

Evidencia principal:

- `.itconfig/validation-20260724-233314.json`: terreno multipass selectivo;
- `.itconfig/validation-20260724-234254.json`: once parejas combinadas;
- `.itconfig/validation-20260724-235249.json`: 15 parejas combinadas;
- `.itconfig/validation-20260724-235600.json`: pareja VS/PS exacta;
- `.itconfig/dx12-terrain-multipass-fixed-20260724.jpg`;
- `.itconfig/dx12-eleven-pairs-combined-20260724.jpg`;
- `.itconfig/dx12-fifteen-pairs-camera-angle-20260724.jpg`.

La familia reflejada `77EC9C4A77F77E37` continúa implementada pero fuera del
conjunto autoritativo. Una prueba amplia mostró siluetas negras en mobiliario
y posibles huecos arquitectónicos; debe repetir una comparación dedicada
antes de promover sus dos pixel shaders.

El siguiente frente es validar las diez parejas programables restantes y,
finalmente, separar por estado los draws de función fija `VS=0/PS=0`.

### Etapa 6p: veinte parejas programables estables

La ampliación directa de 15 a 23 parejas produjo un error de dispositivo y,
después del fallo de reproducción, un acceso nulo secundario desde
`RSSetHazeCoordinates`. Se revirtió la promoción masiva y se probó cada pareja
por separado con selector VS/PS exacto.

Cinco parejas adicionales quedaron autorizadas:

- `4B5B9BE51A8EFA7E / B5BD45A8BA08F65B`;
- `3C15F8B8DEBF2EC8 / B019C7A089216D68`;
- `F6F2AA8EA79D28BC / 000D90AFD69D7DA9`;
- `77EC9C4A77F77E37 / 4BDD3F424E9CB4D1`;
- `77EC9C4A77F77E37 / D9A8DB50746FD55D`.

La familia `77EC9C4A77F77E37` se volvió a evaluar después de estabilizar el orden
de composición. El defecto anterior de las sillas no se reprodujo y el
usuario confirmó que ese mobiliario ya se visualiza correctamente.

La regresión conjunta de las 20 parejas terminó aprobada, con cierre limpio,
27 muestras responsivas de 34, cero rechazos y cero eventos de aplicación,
pantalla o dispositivo. Procesó hasta 144 000 triángulos DX12 por cuadro y
redujo el fallback observado al final del recorrido a 11 draws. Una segunda
ejecución de 90 segundos volvió a aprobar 43 muestras; además, el usuario
recorrió todos los ángulos de cámara relevantes y confirmó que el suelo ya no
desaparece ni se vuelve negro.

Evidencia:

- `.itconfig/validation-20260725-002053.json`: 18 parejas;
- `.itconfig/validation-20260725-002218.json`: familia `77EC` aislada;
- `.itconfig/validation-20260725-002451.json`: 20 parejas combinadas;
- `.itconfig/validation-20260725-002936.json`: regresión adicional del terreno;
- `.itconfig/dx12-eighteen-pairs-20260725.jpg`;
- `.itconfig/dx12-77ec-retest-20260725.jpg`;
- `.itconfig/dx12-twenty-pairs-20260725.jpg`;
- `.itconfig/dx12-terrain-low-angle-20260725.jpg`;
- `Engine.dll` SHA-256
  `80CBBC6F3CCD4C53B061F425A0B202990A0D5ED1A5E49AD64CAA6EA31A70DAFE`.

Quedan cinco parejas programables implementadas pero no observadas en la
escena actual y, por lo tanto, todavía no autorizadas:

- `81B4D7CBDC31B625 / 93145689E4FC29A7`;
- `E1AA07F9418DE37E / 3CA2992E872AB363`;
- `3217ECE2D2C1D96A / F91A55624E94D8A1`;
- `3217ECE2D2C1D96A / 5B3BD26F0B904B3D`;
- `7873727C8ED9D187 / 77162620F6305229`.

El siguiente frente de implementación es `VS=0/PS=0`. En la plaza actual sus
draws se rechazan principalmente por transparencia fixed-function, índices y
arrays no capturados, además de un caso que cruza el plano `W=0`.

### Etapa 6q: transparencias fixed-function y subbuffers dinámicos

La migración de `VS=0/PS=0` se separó por modo de captura y mezcla para evitar
autorizar todas las transparencias como un único bloque. El laboratorio permite
seleccionar capas opacas, transparentes o ambas, además de filtrar por modo de
blend y ancho de textura. Esto permitió ejercer por separado:

- alpha convencional (`SRC_ALPHA / INV_SRC_ALPHA`) para polvo y humo;
- shade (`DEST_COLOR / SRC_COLOR`) para sombra e indicador de movimiento;
- suma (`ONE / ONE`);
- suma con alpha (`SRC_ALPHA / ONE`).

Se corrigieron dos contratos distintos. Primero, `_iTexPass` cuenta los arrays
UV preparados desde el último bloqueo de vértices y puede conservar un valor
mayor que las etapas realmente activas. El traductor ahora deriva la cantidad
de texturas de la secuencia `D3DTSS_COLOROP` hasta `D3DTOP_DISABLE`. Esto
eliminó la pasada blanca que aparecía al acercar la cámara y mantuvo visible el
humo con su alpha correcto.

Segundo, el renderer de mundo escribe posiciones, UV y colores directamente
mediante `gfxLockSubBuffer`. Esa ruta no notificaba al batch DX12, que podía
reutilizar arrays de una geometría anterior. Se agregó una captura común antes
de `Unlock` para posición, normal, tangente, pesos, UV y color. La sombra
`256x256` y el indicador de movimiento `1x1` vuelven a usar su geometría y sus
coordenadas reales; desapareció el rombo negro sin ocultar contenido válido.
La captura se activa únicamente cuando el selector de familia es `fixed`.
Aplicarla globalmente a todas las familias incrementó el volumen de trabajo de
la ruta de producción y terminó en una excepción de acceso bajo carga.

Durante el aislamiento se probó omitir el pase plano `1x1`, pero se retiró: el
usuario confirmó que la cuña verde estable es el indicador de movimiento
correcto. La solución final conserva ese draw y corrige únicamente los datos
obsoletos.

Validación:

- `.itconfig/validation-20260725-013313.json`: shade `256x256`, 103 segundos,
  cierre limpio y confirmación manual en movimiento;
- `.itconfig/validation-20260725-013804.json`: shade `1x1` renderizado, sin
  omisión especial;
- `.itconfig/validation-20260725-013937.json`: todas las transparencias fixed
  combinadas, cierre limpio, cero eventos de aplicación, pantalla o
  dispositivo;
- `.itconfig/validation-20260725-014434.json`: ensayo descartado con captura de
  subbuffers global; falló bajo carga con evento NVIDIA 153 y excepción
  `0xc0000005` en `Engine.dll`;
- `.itconfig/validation-20260725-014756.json`: regresión de producción con la
  captura limitada a `fixed`, 70 segundos, resultado satisfactorio y cero
  eventos de aplicación, pantalla o dispositivo;
- `.itconfig/validation-20260725-015002.json`: regresión visual aislada de
  `fixed`; el render permaneció correcto durante el juego y el usuario lo
  confirmó, aunque el cierre automático del harness produjo una excepción de
  acceso. Por este motivo no se usa como autorización de promoción;
- `.itconfig/dx12-shadow-uv-capture-fix-motion-20260725.png`;
- `.itconfig/dx12-flat-shade-real-uv-20260725.png`;
- `.itconfig/dx12-fixed-transparent-final-20260725.png`;
- confirmación visual del usuario: polvo, sombra e indicador de movimiento
  correctos;
- `Engine.dll` SHA-256
  `5593D88C3B90432D5E578B14DA848212F958FCE01E12D60D8734EF9AEEBED243`.

Las capas transparentes fixed permanecen detrás de la compuerta de laboratorio
hasta completar una regresión en más mapas. El siguiente paso es promover sus
variantes ya verificadas de forma selectiva y continuar con los estados fixed
que aún caen en fallback.

### Etapa 6r: inventario offline del contenido y Shaders.dll

Se agregó `tools/dx12_shader_inventory` para sustituir la exploración manual de
mapas como mecanismo primario de descubrimiento. El analizador no abre el
juego: procesa todos los manifiestos `.sha`, extrae descriptores y ensamblador
desde la `Shaders.dll` instalada, busca referencias ASCII y UTF-16LE en cada
byte de `client/Data`, inventaría shaders internos de C++ y cruza el resultado
con el catálogo de familias DX12.

La carga directa de `Engine.dll` fuera del cliente se bloqueaba durante la
inicialización de objetos globales. Se resolvió con un host x64 mínimo que
implementa solamente la ABI importada por `Shaders.dll`; las funciones de
render quedan como stubs y `CTString` conserva la semántica necesaria para
recibir el código. El host se compila en `.itconfig`, nunca reemplaza el motor
del juego y no crea dispositivos D3D9 ni D3D12.

Resultado del snapshot
`6c66909a2ade21ce7adcc7dbac65c9e5b0fe0a75ecef64084648a8cd70552ffe`:

- 48 694 archivos y 4 982 462 079 bytes leídos sin errores;
- 13 679 modelos `.bm` con al menos una referencia y 13 931 relaciones
  modelo/manifiesto;
- cero referencias a manifiestos desconocidos;
- 25 manifiestos catalogados, 13 referenciados explícitamente;
- 18 vertex shaders y 48 pixel shaders únicos extraídos de la DLL;
- 112 parejas de código componibles por manifiesto;
- 26 definiciones de shader internas y 15 sitios de creación directa en C++;
- catálogo DX12 actual: 19 familias VS, 20 familias PS, 25 parejas
  implementadas y 20 validadas.

Se detectaron seis manifiestos cuyos exports no existen en la DLL:
`BaseDS`, `BaseTransparentDS`, `NiceWater`, `ReflectionDS+SpecularDS`,
`ReflectionDS` y `SpecularDS`. Ninguno tiene referencias en el contenido
instalado, por lo que se registran como residuos inactivos y no como familias
pendientes.

El informe reproducible queda en
`.itconfig/dx12-shader-inventory/latest/summary.md`; el JSON completo, el CSV de
referencias y el ensamblador extraído permanecen junto a él. La herramienta,
sus pruebas y sus límites están documentados en
`tools/dx12_shader_inventory/README.md`.

El inventario actual garantiza el superset a nivel de código exportado. La
siguiente extensión debe materializar pesos por vértice, normalización, niebla,
tipo de normal map y declaración D3D9 para obtener los fingerprints finales.
Una prueba aislada del mismo ensamblador ya reprodujo exactamente
`BFDAAD52F7C28AAF`, `3C15F8B8DEBF2EC8`, `F6F2AA8EA79D28BC` y
`000D90AFD69D7DA9`, confirmando que esa correlación puede hacerse totalmente
offline.

### Etapa 6s: fingerprints exactos y correlación total

El analizador offline ahora reproduce la ruta completa que genera la identidad
runtime de cada shader. Lee los fragmentos de `ShaderCode.h`, materializa entre
cero y cuatro pesos, ambas configuraciones de normalización, las unidades de
niebla realmente declaradas por cada shader y el ensamblado tangent-space.
Después replica la conversión `CompileVertexProgram_D3D`, ensambla mediante
`D3DXAssembleShader`, agrega la estructura binaria producida por
`GetShaderDeclaration_D3D9` y aplica FNV-1a 64 exactamente en el mismo orden que
`DirectX12Legacy3DCommandBatch`.

El uso de D3DX9 queda limitado a esta herramienta de desarrollo. No se crea un
dispositivo gráfico y no se agrega ninguna dependencia D3D9 a la ejecución
DX12 del cliente.

También se agregaron extractores para las cinco familias internas
`TRShader_*` y para los shaders creados directamente por
`CTraceEffect`, `CShockWaveEffect` y `CSplineBillboardEffect`. Sin esas fuentes
el cruce alcanzaba 20 de 25 parejas; al incluirlas quedó completo:

- 457 variantes VS compiladas, 322 fingerprints VS únicos;
- 84 variantes PS compiladas, 51 fingerprints PS únicos;
- 805 parejas exactas candidatas después de compatibilizar niebla;
- 19 de 19 fingerprints VS y 20 de 20 fingerprints PS del catálogo
  reproducidos;
- 25 de 25 parejas implementadas y 20 de 20 parejas validadas correlacionadas.

D3DX9 rechazó tres combinaciones `FT_NON_OPAQUE` exportadas históricamente:
dos de `Detail_Specular` y una de `MultiLayer`. Usan registros `t#` en
instrucciones aritméticas de `ps_1_4`; ninguna pertenece al catálogo DX12
actual. Se conservan en `assembly_errors` para no confundir código presente en
la DLL con una variante ejecutable.

Las pruebas automatizadas verifican, entre otros contratos, que el ensamblado
offline vuelve a producir `BFDAAD52F7C28AAF` para el VS Base normalizado y
`000D90AFD69D7DA9` para su PS.

### Etapa 6t: validación determinista de las cinco parejas restantes

Se promovieron las cinco parejas que permanecían implementadas pero no
autorizadas:

- `Shadow.sha`: `81B4D7CBDC31B625 / 93145689E4FC29A7`;
- `NoShadow.sha`: `E1AA07F9418DE37E / 3CA2992E872AB363`;
- NormalMap rígido con niebla opaca:
  `3217ECE2D2C1D96A / F91A55624E94D8A1`;
- NormalMap rígido con niebla no opaca:
  `3217ECE2D2C1D96A / 5B3BD26F0B904B3D`;
- NormalMap animado especular:
  `7873727C8ED9D187 / 77162620F6305229`.

Shadow y NoShadow se validaron sobre sus render targets auxiliares reales. La
ruta de captura DX12 ahora vacía el batch 3D antes de cambiar de destino y
acepta las pasadas auxiliares seleccionadas por la prueba, sin promoverlas de
forma general.

NormalMap necesitó dos hooks optativos porque `ModelHolder3` siempre entra por
la ruta SKA con cuatro pesos y los mapas recorridos no garantizan niebla
activa. Las variables
`LASTCHAOS_DX12_3D_TEST_FORCE_NORMALMAP_FOG` y
`LASTCHAOS_DX12_3D_TEST_FORCE_NORMALMAP_RIGID` preparan esos estados sólo
durante una prueba explícita. No crean alias de fingerprints, no se activan en
una ejecución normal y ejercitan los bytecodes D3D9 exactos que consume el
reemplazo DX12.

Durante la primera escena controlada de Dungeon1 el personaje quedó flotando.
La causa no era el renderer: el arnés utilizaba el centro geométrico del sector
con niebla como si fuera una superficie caminable. Un sector puede abarcar
varios niveles y su centro vertical no coincide con el suelo. La corrección
conserva el punto válido de entrada del personaje y relaciona únicamente el
fixture de prueba con el sector de niebla. Los modelos grandes también se
separan de la cámara; para evidencia visual se prefiere la gárgola pequeña.

Evidencia:

- `.itconfig/validation-20260725-235135.json`: Shadow, 46.128 envíos y
  13.254.624 triángulos;
- `.itconfig/validation-20260725-235532.json`: NoShadow, 21.194 envíos;
- `.itconfig/validation-20260726-003647.json`: NormalMap animado especular;
- `.itconfig/validation-20260726-014555.json`: NormalMap rígido y niebla
  opaca exactos, 3.136 envíos y 1.081.920 triángulos;
- `.itconfig/validation-20260726-014949.json`: NormalMap rígido y niebla no
  opaca exactos, 1.382 envíos y 972.237 triángulos;
- `.itconfig/dx12-normalmap-rigid-fog-nonopaque-exact.png`;
- `.itconfig/validation-20260726-015229.json`: regresión sin hooks ni
  selectores, 137.966 envíos DX12, 54.297.133 triángulos, cero rechazos en la
  pasada principal y cero fallos de dispositivo;
- `.itconfig/dx12-25-of-25-regression.png`.

El catálogo queda en 25 de 25 parejas implementadas y 25 de 25 autorizadas
para reemplazo estable.

### Etapa 6u: eliminación del fallback de profundidad del primer frame

La primera regresión 25/25 todavía registraba 55 fallbacks bajo el contador
combinado de captura/profundidad. No correspondían a familias desconocidas:
durante el primer frame el backend aún no consideraba disponible la
profundidad, aunque el dispositivo D3D9On12 ya había creado su depth surface.
La disponibilidad se copiaba desde el resultado del frame anterior y obligaba
a calentar el renderer con un frame de geometría D3D9.

El backend ahora consulta una vez por frame la depth surface asociada al
dispositivo. Esto permite capturar desde el primer draw y, al enviar el lote,
usar la profundidad interoperable adquirida o el depth buffer privado DX12
existente. La consulta conserva la referencia COM únicamente durante la
comprobación y no altera el estado del dispositivo.

Validación:

- `.itconfig/validation-20260726-015810.json`: 128.029 envíos DX12 y
  47.874.614 triángulos;
- cero envíos rechazados y ninguna línea de fallback durante toda la prueba;
- cero eventos de aplicación, pantalla o dispositivo;
- cierre controlado y captura completada;
- `.itconfig/dx12-zero-depth-warmup-fallback.png`.

### Etapa 6v: profundidad nativa para render targets auxiliares

`CRenderTexture` creaba una superficie D16 de D3D9 para cada textura usada
como render target, incluso cuando Shadow y NoShadow se sustituían por completo
en DX12. Esas pasadas ya utilizan `CDirectX12DepthBuffer`, que crea un recurso
de profundidad compatible con el tamaño del destino, lo limpia y lo enlaza a
la command list nativa.

El backend expone ahora una política única para el depth auxiliar. En modo
replace, `CRenderTexture` omite `CreateDepthStencilSurface`, enlaza profundidad
nula al dispositivo legado y limita el clear D3D9 al color interoperable. En
los modos de comparación conserva la superficie D16, porque allí los draws
D3D9 siguen siendo visibles junto con DX12.

Validación:

- `.itconfig/validation-20260726-020343.json`: Shadow exacto, 19.416 draws y
  5.579.376 triángulos, sin fallbacks;
- `.itconfig/dx12-shadow-native-offscreen-depth.png`;
- `.itconfig/validation-20260726-020606.json`: NoShadow exacto, 20.574 draws,
  sin fallbacks;
- `.itconfig/dx12-noshadow-native-offscreen-depth.png`;
- `.itconfig/validation-20260726-020746.json`: regresión completa, 83.349
  draws y 89.743.412 triángulos;
- cero rechazos, fallbacks y eventos de aplicación, pantalla o dispositivo;
- `.itconfig/dx12-native-offscreen-depth-regression.png`.

### Etapa 6w: identidad nativa para render textures y bloom

`CRenderTexture` conserva una textura D3D9 únicamente como adaptador para los
consumidores que todavía no fueron migrados. Su identidad nativa es ahora un
`DirectX12RenderTextureHandle` estable, administrado por el backend sin exponer
un puntero `IDirect3DTexture9` a las pasadas DX12.

`CDirectX12Texture` es dueño conjunto del recurso de GPU, su SRV y su RTV. Los
RTV persistentes se asignan desde un heap compartido y reemplazan los heaps
privados y las vistas recreadas por bloom. El registro de interoperabilidad
mantiene la asociación con D3D9 sólo en el borde para que sombras y reflejos
pendientes puedan seguir resolviendo la misma textura durante la transición.

Bloom recibe directamente las tres `CDirectX12Texture`. La captura de escena
usa `CDirectX12RenderTargetManager::CopyCurrentColorTo`, que transiciona y copia
el render target sincronizado hacia el recurso fuente nativo; ya no necesita
una superficie D3D9 de destino ni localizar los filtros por puntero legado.

Validación:

- build `LCRelease|x64` de Engine completado;
- cámara verificada:
  `.itconfig/dx12-camera-captures/camera-repro-20260727-172321.json`;
- captura:
  `.itconfig/dx12-camera-replays/native-render-texture-bloom.png`;
- terreno opaco, verde y con caminos; sin blanqueo ni geometría de fondo
  visible a través del piso.

El terreno continúa explícitamente en su fallback D3D9On12 y no forma parte de
este corte.

### Etapa 6x: shaders DXIL precompilados con DXC

Los shaders nativos dejaron de vivir como strings C++ y ya no se compilan
durante `CDirectX12PipelineCache::Initialize`. Las siete fuentes están ahora en
`Graphics/Shaders/DirectX12` y conservan el mismo contrato de entradas,
constantes, texturas y samplers.

El target `CompileDirectX12Shaders` de Engine ejecuta
`scripts/compile-dx12-shaders.ps1` antes de `ClCompile`. El generador localiza
DXC en el Windows SDK seleccionado, compila 15 entradas `vs_6_0`/`ps_6_0`,
elimina debug y reflection de los contenedores DXIL y produce un header
mecánico dentro de `$(IntDir)Generated`. MSBuild declara fuentes, script y
header como `Inputs`/`Outputs`, por lo que una compilación incremental no
invoca DXC cuando nada cambió.

`DirectX12NativeShaderCatalog` es la única frontera con el header generado.
`DirectX12PipelineCache` selecciona IDs tipados y recibe
`D3D12_SHADER_BYTECODE`; ya no conserva `ID3DBlob`, incluye
`d3dcompiler.h`, enlaza `d3dcompiler.lib` ni llama `D3DCompile`.

Validación estática:

- 15 blobs DXIL generados con el SDK `10.0.26100.0`;
- build `LCRelease|x64` de Engine completado;
- segundo build incremental sin regeneración DXC;
- `Engine.dll` sin imports de `D3DCompile` ni `d3dcompiler`;
- reproducción visual con
  `.itconfig/dx12-camera-captures/camera-repro-20260727-174021.json`;
- captura `.itconfig/dx12-camera-replays/dxc-offline-native-shaders.png`:
  terreno verde y opaco, caminos, geometría, UI y bloom sin regresiones.

### Etapa 6y: caché nativo de texturas muestreadas

Las texturas administradas que consumen los draw calls DX12 dejaron de formar
parte del estado interno de `DirectX12InteropTextureManager`.
`DirectX12SampledTextureCache` es ahora responsable de crear el recurso
`ID3D12Resource`, conservar su SRV y entregar directamente el descriptor GPU.
El gestor de interoperabilidad queda limitado a recursos `D3DPOOL_DEFAULT`,
render targets y el retorno explícito a D3D9On12.

Mientras el cargador de assets continúe creando su pareja D3D9, esa interfaz se
usa solamente como identidad y fuente transitoria de los mipmaps. Los uploads
legados notifican al backend para precalentar inmediatamente la versión DX12
dentro del command list abierto. `Acquire` mantiene una creación bajo demanda
para texturas cargadas antes del primer frame.

Al refrescar o destruir una textura, el recurso nativo anterior no se libera
inmediatamente: se retira en el slot del frame actual y se destruye cuando el
backend vuelve a ese slot después de esperar su fence. Esto permite actualizar
assets dinámicos sin liberar memoria que todavía pueda estar en uso por la GPU.

Este corte no cambia la selección del terreno ni elimina aún la creación
D3D9 de los assets. Prepara una frontera estable para que el cargador escriba
los subrecursos DX12 directamente.

Validación:

- build `LCRelease|x64` completo de `Nksp` y sus dependencias;
- log `client/Nksp.log` con activación del caché nativo y sin errores DX12;
- cámara verificada
  `.itconfig/dx12-camera-captures/camera-repro-20260727-181413.json`;
- captura
  `.itconfig/dx12-camera-replays/native-sampled-texture-cache-final.png`:
  terreno, caminos, edificios, personaje, UI y efectos con sus texturas
  correctas, sin recursos blancos, transparentes ni desactualizados.

### Etapa 6z: uploads de assets directamente a subrecursos DX12

Las cargas normales de texturas ya no reconstruyen su recurso nativo leyendo
la copia D3D9 con `LockRect`. `DirectX12TextureUploadSource` prepara
subrecursos propiedad del backend directamente desde:

- la cadena RGBA de mipmaps generada por `CTextureData`;
- el blob de mipmaps DXT1, DXT3 o DXT5 almacenado en el asset.

El preparador conserva el formato y el component mapping correspondientes:
BGRA8, B5G6R5, B5G5R5A1, luminancia, luminancia-alpha y BC1/BC2/BC3. Los
formatos A4R4G4B4/X4R4G4B4, que DXGI no expone como textura muestreable,
se expanden a BGRA8 durante la preparación.

`Gfx_wrapper` entrega los datos CPU al backend inmediatamente después de
actualizar la pareja D3D9. El caché crea el `ID3D12Resource`, copia todos los
subrecursos mediante `DirectX12UploadManager` y publica su SRV sin releer la
textura legada. `Acquire` conserva temporalmente la ruta `LockRect` para assets
precargados antes de abrir el primer frame o para formatos todavía no
enrutados.

La pareja D3D9 aún se mantiene como identidad y respaldo porque el terreno y
otros draws no promovidos continúan ejecutándose por D3D9On12. El siguiente
corte puede separar handles nativos de identidades D3D9 por familia una vez que
esos consumidores hayan sido promovidos.

Validación:

- build completo `LCRelease|x64` de `Nksp`;
- log con activación independiente de uploads CPU RGBA y DXT directos, sin
  errores DX12;
- cámara verificada
  `.itconfig/dx12-camera-captures/camera-repro-20260727-183207.json`;
- captura
  `.itconfig/dx12-camera-replays/native-direct-texture-uploads-final.png`:
  terreno verde y opaco, caminos, edificios, personaje, UI y efectos
  correctamente texturados.

### Corrección del cielo visible a través del terreno (2026-07-26)

El defecto que parecía agua era el color del cielo conservado por la primera
pasada del terreno. La geometría sí se rasterizaba: el shader multipass entrega
color premultiplicado y usa `ONE/SRC_ALPHA`, pero la primera capa no puede
depender del contenido previo del render target.

La solución configura únicamente la primera capa de cada bloque como escritura
opaca. Las capas restantes conservan `ONE/SRC_ALPHA`, por lo que la composición
de máscaras sigue siendo la original. También se limita `z` a `w` para los
vértices proyectados que D3D9 dejaba unas diezmilésimas fuera del plano lejano.
El log informa `corregidosLejano` y confirma que `fueraLejano` queda en cero.

La captura de ventanas usa ahora `PrintWindow(PW_RENDERFULLCONTENT)` para no
confundir una ventana superpuesta con el resultado del juego. La regresión
`.itconfig/validation-20260726-163636.json` aprobó 47 muestras, 40 responsivas,
309 draws DX12 máximos y cero eventos de aplicación o pantalla. La referencia
visual es `.itconfig/terrain-opaque-pso-regression.png`.

### Terreno nativo autoritativo

El reemplazo integral ya no requiere una variable de laboratorio para el
terreno. Las tres familias proyectadas y su pasada fixed-function de máscara
de profundidad se capturan y se suprimen juntas, conservando el orden original
en el command stream DX12.

Los perfiles de diagnóstico que no activan el reemplazo integral mantienen
todo el terreno en D3D9On12. El fixed-function general sigue aislado: este
corte solo autoriza la máscara identificada por escritura de color nula,
escritura de profundidad activa y alpha-test activo.

### Captura reproducible de posición y cámara

Una cuenta normal puede registrar una vista defectuosa sin comandos GM ni
tráfico hacia el servidor. Con el juego abierto en el ángulo exacto se ejecuta:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\capture-dx12-view.ps1 -Name piso-fallo
```

El script crea una solicitud en `.itconfig`. El cliente la consume durante el
siguiente draw de terreno y escribe
`.itconfig/dx12-camera-captures/camera-piso-fallo.json`. El archivo contiene
zona, área, capa, posición del personaje, orientación relativa de cámara,
matrices D3D9, viewport y las 96 constantes `float4` del vertex shader que
generaron el terreno. La captura es completamente local y no requiere nivel GM.

Para repetir automáticamente una vista ya registrada se usa:

```powershell
powershell -ExecutionPolicy Bypass -File `
  .\scripts\run-dx12-camera-repro.ps1 `
  -FixturePath .itconfig\dx12-camera-captures\camera-piso-fallo.json
```

El lanzador toma las credenciales de `.itconfig/lastchaos-test.settings.psd1`,
instala la última compilación local, inicia sesión, selecciona servidor, canal
y personaje, y restaura la posición y la cámara al entrar al mundo. Espera un
draw real de terreno antes de guardar el PNG en
`.itconfig/dx12-camera-replays`. Las credenciales no se imprimen ni se guardan
en el repositorio.

Los mismos datos pueden pasarse directamente a `Jugar-Espanol.cmd` mediante
`+testplayerplacement`, `+testviewpoint`, `+testcameraangle`,
`+testworldviewdelay`, `+testworldviewhold` y `+testcapture`. Los lanzadores
intermedios reenvían todos los argumentos a `Nksp`.

### Arranque negro desde el lanzador en español (2026-07-26)

`Jugar-Espanol.cmd` configuraba correctamente los recursos en español, pero
delegaba en `Jugar.cmd` sin seleccionar el perfil autoritativo de presentación.
El motor avanzaba desde introducción hasta login y podía incluso conectarse,
mientras la ventana conservaba el backbuffer negro del modo de comparación.

`Jugar.cmd` configura ahora `LASTCHAOS_DX12_UI_COMPARE=replace` y
`LASTCHAOS_DX12_3D_REPLACE_ALL=enabled` antes de iniciar `Nksp`. La prueba desde
el mismo lanzador alcanzó `eSTAGE_GAMEPLAY`, mantuvo el proceso responsivo y
presentó la UI nativa sin fallbacks. La traza
`Diagnostico de arranque: etapa N detectada` permite distinguir en futuros
incidentes un bloqueo de etapa de un fallo de presentación.

### Cobertura completa del inventario de shaders observado

Las 28 parejas VS/PS observadas en el inventario de contenido ya tienen una
traducción semántica DX12. Las dos últimas familias no reutilizan huellas de
otras rutas:

- `SKINNED_LIT_DETAIL` genera el segundo juego de UV aplicando la escala de la
  constante VS 12 al UV base;
- `SKINNED_TANGENT_PROJECTED` conserva skinning, base tangente y la coordenada
  proyectada de la constante VS 18 usada por NormalMap con niebla.

La familia Detail se validó con
`Data\Monster\LeadingBand\Nor_LB_Moving.smc`: 937 draws y 277 352 triángulos
nativos, sin errores de dispositivo o aplicación. NormalMap proyectado se
validó en zona 9 con `Data\World\Dungeon1\Ska\d1_gagoil_001.smc`: 3 950 draws
y 1 362 750 triángulos nativos. La captura retardada confirmó escenario,
personaje, piso, iluminación y texturas correctos; la captura inmediata negra
correspondía al intervalo de carga y no a un fallo de render.

El recorrido integral reveló además la variante
`58BF46CA623CB0F3 / 0000000000000000`: vertex color programable con una etapa
de textura fixed-function. Se promueve mediante la familia `PS_FIXED` existente,
sin duplicar shaders. Su prueba selectiva procesó 102 draws y 2 896 triángulos
nativos, sin eventos de dispositivo, aplicación o pantalla.

### Handles nativos en los command streams promovidos

Los rangos capturados de UI y 3D usan ahora el handle generacional DX12 como
identidad canónica de cada textura. Cuando el caché muestreado o el registro de
render textures ya expone un handle válido, el rango deja de retener el
`IDirect3DTexture9` y no incrementa su referencia COM.

El puntero D3D9 se conserva únicamente para recursos que todavía no han sido
materializados en DX12 y necesitan la ruta de adquisición diferida. La
selección de SRV, la detección de realimentación RTV/SRV y la agrupación de
rangos funcionan directamente con `DirectX12TextureHandle` o
`DirectX12RenderTextureHandle`. De esta forma un refresh puede sustituir la
identidad D3D9 sin invalidar comandos nativos ya grabados; el handle anterior
permanece vivo hasta reciclar la fence del frame.

El borde de captura de UI y 3D prepara la identidad nativa justo antes de
grabar el draw. Esto cubre assets precargados antes de inicializar DX12 y
bindings conservados por D3D9 desde un frame anterior: la adquisición diferida
ocurre antes de crear el rango y el command stream nace con el handle.

Validación:

- build completo `LCRelease|x64`;
- telemetría `handle nativo preparado antes de capturar el draw`;
- telemetría independiente de UI y 3D confirmando que no retienen identidades
  COM D3D9;
- cámara verificada
  `.itconfig/dx12-camera-captures/camera-repro-20260728-153829.json`;
- captura
  `.itconfig/dx12-camera-replays/dx12-native-command-handles-verified.png`,
  con terreno, personaje, edificios, efectos y HUD correctos;
- cierre automático del cliente confirmado.

### Frontera de estado 3D sin dispositivo D3D9

La ruta promovida de geometría ya no entrega `IDirect3DDevice9` a
`DirectX12NativeRenderer` ni a `DirectX12Legacy3DCommandBatch`. El adaptador de
compatibilidad produce una única `CDirectX12LegacyDrawState` inmutable que
agrupa:

- matrices de mundo, vista, proyección y transformaciones de textura;
- viewport, profundidad, rasterización, mezcla y alpha test;
- sampler, combinadores fixed-function y constantes de shaders;
- identidades temporales y declaración necesarias para resolver la familia de
  shader, además de hasta cuatro bindings de textura.

Los aliases COM del snapshot tienen vida acotada a la llamada de captura. Los
command streams continúan reteniendo únicamente handles generacionales cuando
el recurso ya fue materializado en DX12. Los vértices, normales, tangentes,
pesos, coordenadas UV, colores e índices no se consultan al dispositivo: se
mantienen en las copias CPU que alimentan directamente los buffers DX12.

La captura determinista de cámara también consume las matrices y el viewport
del snapshot, eliminando sus consultas tardías al dispositivo desde el command
batch. Esto deja todas las lecturas D3D9 de un draw concentradas en el adaptador
de `DirectX12Backend`, listo para sustituirse por setters nativos sin modificar
el renderer ni sus command streams.

Validación:

- `Engine.vcxproj` compilado como `LCRelease|x64`;
- hash SHA-256 idéntico entre el artefacto compilado, el staging moderno y el
  `Engine.dll` instalado;
- telemetría `estados, constantes y bindings cruzan como snapshot`;
- ausencia estática de `IDirect3DDevice9` y de consultas `Get*` en
  `DirectX12NativeRenderer` y `DirectX12Legacy3DCommandBatch`;
- cámara verificada
  `.itconfig/dx12-camera-captures/camera-repro-20260728-160324.json`;
- captura
  `.itconfig/dx12-camera-replays/dx12-native-state-snapshot-verified.png`,
  con terreno, personaje, edificios, efectos y HUD correctos;
- cierre automático del cliente confirmado.

### Estado 3D persistente alimentado por setters

`CDirectX12LegacyDrawState` dejó de ser una instantánea reconstruida al entrar
en `QueueLegacy3DIndexedDraw`. Ahora pertenece al backend durante toda la vida
del contexto y se actualiza en el mismo punto donde el motor escribe cada
matriz, viewport, render state, sampler, combinador fixed-function, shader,
constante y textura.

La ruta de draw consume este estado persistente por referencia. Por lo tanto,
el adaptador ya no ejecuta `GetTransform`, `GetViewport`, `GetRenderState`,
`GetSamplerState`, `GetTextureStageState`, `GetVertexShader`,
`GetPixelShader`, `GetVertexShaderConstantF`, `GetPixelShaderConstantF`,
`GetVertexDeclaration` ni `GetTexture` para capturar un draw. Los command
streams y el renderer nativo mantienen la frontera de handles DX12 introducida
en la etapa anterior.

También se conectaron las escrituras directas que no atravesaban los wrappers
principales: matriz de proyección del plano de recorte, matriz de vista de
billboards, estados de bump mapping, pases de haze/fog, recreación de texturas
y liberación del vertex shader activo. El tracker se reinicia junto con el
contexto y conserva los mismos valores iniciales que `InitContext_D3D`.

Validación:

- `Engine.vcxproj` compilado como `LCRelease|x64`;
- ausencia estática de las consultas D3D9 anteriores en
  `DirectX12Backend.cpp` y `DirectX12LegacyDrawState.cpp`;
- hash SHA-256 idéntico para `Engine.dll` compilado, staging e instalación;
- telemetría
  `QueueLegacy3DIndexedDraw no consulta D3D9`;
- dos reproducciones consecutivas contra `127.0.0.1`, ambas con captura de
  terreno y `ClientClosed=True`;
- cámaras verificadas
  `.itconfig/dx12-camera-captures/camera-repro-20260728-173108.json` y
  `.itconfig/dx12-camera-captures/camera-repro-20260728-173155.json`;
- capturas
  `.itconfig/dx12-camera-replays/dx12-setter-fed-state-verified.png` y
  `.itconfig/dx12-camera-replays/dx12-setter-fed-state-verified-repeat.png`,
  con terreno, personaje, edificios, efectos y HUD correctos.

### Destino y buffers 3D autoritativos por estado nativo

La clasificación del destino dejó de comparar identidades COM obtenidas con
`GetRenderTarget` en cada draw. `DirectX12Backend` conserva ahora un
`DirectX12LegacyRenderTargetKind` explícito, actualizado por los puntos reales
que enlazan el backbuffer de presentación o una render texture auxiliar. Se
eliminaron `ClassifyLegacyRenderTarget`, la identidad retenida del backbuffer y
el parámetro `IDirect3DDevice9` de `QueueLegacy3DIndexedDraw` y
`PrepareLegacy3DDepthClear`.

La procedencia dinámica o estática de la geometría también forma parte de
`CDirectX12LegacyDrawState`. Los setters de posición la actualizan junto con
las copias CPU, por lo que el draw ya no recibe un indicador externo de binding.
En el envío DX12, los vertex e index buffers agregados se resuelven desde sus
handles generacionales `DirectX12VertexBufferHandle` y
`DirectX12IndexBufferHandle` antes del upload y de `IASetVertexBuffers` /
`IASetIndexBuffer`; los punteros quedan limitados a la propiedad interna.

Durante la validación apareció de forma reproducible una carrera previa en
`CDirectX12SampledTextureCache::Remove`: el inicio de frame, los reemplazos y
los retiros podían modificar simultáneamente las colecciones del caché. El
estado del caché usa ahora una sección crítica con guarda RAII, incluyendo las
rutas anidadas de `Replace`/`Remove`. El primer arranque de un build nuevo y el
arranque consecutivo terminaron sin la violación de acceso anterior.

Validación:

- `Engine.vcxproj` compilado como `LCRelease|x64`;
- ausencia de `ClassifyLegacyRenderTarget`, de la identidad COM del backbuffer
  y de parámetros de dispositivo en la captura 3D;
- telemetría
  `vertex/index buffers resueltos por handles generacionales`;
- hash SHA-256 idéntico para `Engine.dll` compilado, staging e instalación;
- dos reproducciones locales con `ClientClosed=True`;
- cámaras verificadas
  `.itconfig/dx12-camera-captures/camera-repro-20260728-175326.json` y
  `.itconfig/dx12-camera-captures/camera-repro-20260728-175417.json`, más la
  comprobación final del build
  `.itconfig/dx12-camera-captures/camera-repro-20260728-175622.json`;
- capturas
  `.itconfig/dx12-camera-replays/dx12-native-target-buffer-bindings-verified.png`
  y
  `.itconfig/dx12-camera-replays/dx12-native-target-buffer-bindings-repeat.png`,
  más
  `.itconfig/dx12-camera-replays/dx12-native-target-buffer-bindings-final.png`,
  con terreno, personaje, edificios, efectos y HUD correctos.
