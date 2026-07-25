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
