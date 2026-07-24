"use strict";

const fs = require("fs");
const path = require("path");

const TEXTURE_WIDTH = 512;
const TEXTURE_HEIGHT = 512;
const BYTES_PER_PIXEL = 4;
const CELL_WIDTH = 12;
const CELL_HEIGHT = 24;
const DRAW_HEIGHT = 14;
const FIRST_CHARACTER = 0x1e;
const CELLS_PER_ROW = 42;

function fail(message) {
  console.error(`Error: ${message}`);
  process.exit(1);
}

function findRawFrame(buffer) {
  const markerOffset = buffer.indexOf(Buffer.from("FRMS", "ascii"));
  if (markerOffset < 0) {
    fail("la fuente no contiene una textura FRMS sin comprimir.");
  }

  const frameOffset = markerOffset + 4;
  const frameSize = TEXTURE_WIDTH * TEXTURE_HEIGHT * BYTES_PER_PIXEL;
  if (frameOffset + frameSize > buffer.length) {
    fail("la textura no tiene el tamaño RGBA 512x512 esperado.");
  }
  return frameOffset;
}

function cellOrigin(characterCode) {
  const sequentialCode = characterCode - FIRST_CHARACTER;
  return {
    x: (sequentialCode % CELLS_PER_ROW) * CELL_WIDTH,
    y: Math.floor(sequentialCode / CELLS_PER_ROW) * CELL_HEIGHT,
  };
}

function pixelOffset(frameOffset, x, y) {
  return frameOffset + (y * TEXTURE_WIDTH + x) * BYTES_PER_PIXEL;
}

function clearCell(buffer, frameOffset, characterCode) {
  const origin = cellOrigin(characterCode);
  for (let y = 0; y < CELL_HEIGHT; y += 1) {
    for (let x = 0; x < CELL_WIDTH; x += 1) {
      const offset = pixelOffset(
        frameOffset,
        origin.x + x,
        origin.y + y,
      );
      buffer[offset] = 0xff;
      buffer[offset + 1] = 0xff;
      buffer[offset + 2] = 0xff;
      buffer[offset + 3] = 0;
    }
  }
}

function copyBaseGlyph(
  buffer,
  frameOffset,
  targetCode,
  baseCharacter,
  { clearTopThrough = -1 } = {},
) {
  clearCell(buffer, frameOffset, targetCode);
  const source = cellOrigin(baseCharacter.charCodeAt(0));
  const target = cellOrigin(targetCode);

  for (let y = 0; y < DRAW_HEIGHT; y += 1) {
    for (let x = 0; x < CELL_WIDTH; x += 1) {
      const sourceOffset = pixelOffset(
        frameOffset,
        source.x + x,
        source.y + y,
      );
      const targetOffset = pixelOffset(
        frameOffset,
        target.x + x,
        target.y + y,
      );
      buffer.copy(
        buffer,
        targetOffset,
        sourceOffset,
        sourceOffset + BYTES_PER_PIXEL,
      );
    }
  }

  for (let y = 0; y <= clearTopThrough; y += 1) {
    for (let x = 0; x < CELL_WIDTH; x += 1) {
      const alphaOffset =
        pixelOffset(
          frameOffset,
          target.x + x,
          target.y + y,
        ) + 3;
      buffer[alphaOffset] = 0;
    }
  }
}

function setGlyphPixels(
  buffer,
  frameOffset,
  characterCode,
  pixels,
) {
  const origin = cellOrigin(characterCode);
  for (const [x, y] of pixels) {
    const offset = pixelOffset(
      frameOffset,
      origin.x + x,
      origin.y + y,
    );
    buffer[offset] = 0xff;
    buffer[offset + 1] = 0xff;
    buffer[offset + 2] = 0xff;
    buffer[offset + 3] = 0xff;
  }
}

const ACCENTS = {
  acuteUpper: [[5, 1], [4, 2]],
  acuteLower: [[5, 2], [4, 3]],
  tildeUpper: [[2, 2], [3, 1], [4, 1], [5, 2], [6, 2], [7, 1]],
  tildeLower: [[2, 4], [3, 3], [4, 3], [5, 4], [6, 4], [7, 3]],
  diaeresisUpper: [[3, 1], [7, 1]],
  diaeresisLower: [[3, 3], [7, 3]],
};

function addDerivedGlyph(
  buffer,
  frameOffset,
  characterCode,
  baseCharacter,
  accent,
  options,
) {
  copyBaseGlyph(
    buffer,
    frameOffset,
    characterCode,
    baseCharacter,
    options,
  );
  setGlyphPixels(
    buffer,
    frameOffset,
    characterCode,
    ACCENTS[accent],
  );
}

function addInvertedGlyph(
  buffer,
  frameOffset,
  characterCode,
  baseCharacter,
) {
  clearCell(buffer, frameOffset, characterCode);
  const source = cellOrigin(baseCharacter.charCodeAt(0));
  const target = cellOrigin(characterCode);

  for (let y = 0; y < DRAW_HEIGHT; y += 1) {
    for (let x = 0; x < CELL_WIDTH; x += 1) {
      const sourceOffset = pixelOffset(
        frameOffset,
        source.x + x,
        source.y + y,
      );
      const targetOffset = pixelOffset(
        frameOffset,
        target.x + (CELL_WIDTH - 1 - x),
        target.y + (DRAW_HEIGHT - 1 - y),
      );
      buffer.copy(
        buffer,
        targetOffset,
        sourceOffset,
        sourceOffset + BYTES_PER_PIXEL,
      );
    }
  }
}

function patchSpanishGlyphs(buffer, frameOffset) {
  addInvertedGlyph(buffer, frameOffset, 0xa1, "!");
  addInvertedGlyph(buffer, frameOffset, 0xbf, "?");

  const definitions = [
    [0xc1, "A", "acuteUpper"],
    [0xc9, "E", "acuteUpper"],
    [0xcd, "I", "acuteUpper"],
    [0xd1, "N", "tildeUpper"],
    [0xd3, "O", "acuteUpper"],
    [0xda, "U", "acuteUpper"],
    [0xdc, "U", "diaeresisUpper"],
    [0xe1, "a", "acuteLower"],
    [0xe9, "e", "acuteLower"],
    [0xed, "i", "acuteLower", { clearTopThrough: 5 }],
    [0xf1, "n", "tildeLower"],
    [0xf3, "o", "acuteLower"],
    [0xfa, "u", "acuteLower"],
    [0xfc, "u", "diaeresisLower"],
  ];

  for (const [
    characterCode,
    baseCharacter,
    accent,
    options,
  ] of definitions) {
    addDerivedGlyph(
      buffer,
      frameOffset,
      characterCode,
      baseCharacter,
      accent,
      options,
    );
  }

  for (const [characterCode, baseCharacter] of [
    [0xc7, "C"],
    [0xe7, "c"],
  ]) {
    copyBaseGlyph(
      buffer,
      frameOffset,
      characterCode,
      baseCharacter,
    );
    setGlyphPixels(buffer, frameOffset, characterCode, [
      [3, 12],
      [2, 13],
    ]);
  }
}

const fontPath = process.argv[2];
if (!fontPath) {
  fail("uso: node patch-latin-font.js <FontLatin0.tex>");
}

const resolvedFontPath = path.resolve(fontPath);
if (!fs.existsSync(resolvedFontPath)) {
  fail(`no existe ${resolvedFontPath}`);
}

const texture = fs.readFileSync(resolvedFontPath);
const frameOffset = findRawFrame(texture);
patchSpanishGlyphs(texture, frameOffset);
fs.writeFileSync(resolvedFontPath, texture);
console.log(
  `Glifos españoles CP1252 instalados en ${resolvedFontPath}`,
);
