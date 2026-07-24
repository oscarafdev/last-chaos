"use strict";

const fs = require("fs");
const path = require("path");

const MAX_STRING_BYTES = 8195;

function readInt32(buffer, offset, label) {
  if (offset + 4 > buffer.length) {
    throw new Error(`Fin inesperado al leer ${label} en offset ${offset}.`);
  }
  return buffer.readInt32LE(offset);
}

function parseLod(filePath, stringCount) {
  const buffer = fs.readFileSync(filePath);
  const declaredRows = readInt32(buffer, 0, "cantidad de filas");
  const declaredMax = readInt32(buffer, 4, "máximo declarado");
  const rows = new Map();
  let offset = 8;

  if (declaredRows < 0 || declaredRows > 1_000_000) {
    throw new Error(`Cantidad de filas inválida en ${filePath}: ${declaredRows}.`);
  }

  // Algunos paquetes heredados declaran más filas que las realmente escritas.
  // Leer hasta EOF evita que el TableLoader replique ese acceso fuera de rango.
  for (let row = 0; offset < buffer.length; row += 1) {
    const index = readInt32(buffer, offset, `índice de fila ${row}`);
    offset += 4;
    const fields = [];

    for (let field = 0; field < stringCount; field += 1) {
      const length = readInt32(buffer, offset, `longitud ${row}:${field}`);
      offset += 4;
      if (length < 0 || offset + length > buffer.length) {
        throw new Error(`Longitud inválida ${length} en ${filePath}, fila ${index}.`);
      }
      fields.push(length === 0 ? null : Buffer.from(buffer.subarray(offset, offset + length)));
      offset += length;
    }
    rows.set(index, fields);
  }

  if (offset !== buffer.length) {
    throw new Error(
      `${filePath} no coincide con el formato de ${stringCount} campos: ` +
      `sobran ${buffer.length - offset} bytes.`,
    );
  }

  return { declaredMax, rows };
}

function detectStringCount(filePath, preferredCount) {
  const validCounts = [];
  for (let count = 1; count <= 8; count += 1) {
    try {
      parseLod(filePath, count);
      validCounts.push(count);
    } catch {
      // Probar el siguiente esquema.
    }
  }

  if (validCounts.includes(preferredCount)) {
    return preferredCount;
  }
  if (validCounts.length === 1) {
    return validCounts[0];
  }
  throw new Error(
    `No se pudo detectar el esquema de ${filePath}; candidatos: ${validCounts.join(", ") || "ninguno"}.`,
  );
}

function isUsable(field) {
  return field && field.length > 0 && field.length <= MAX_STRING_BYTES;
}

function getTranslatedField(fields, fieldIndex, outputCount) {
  if (fields.length === outputCount) {
    return fields[fieldIndex];
  }
  if (outputCount === 2 && fields.length > 2) {
    return fields[fieldIndex === 0 ? 0 : fields.length - 1];
  }
  return fields[fieldIndex];
}

function mergeLod(source, translation, stringCount) {
  const indexes = [...new Set([...source.rows.keys(), ...translation.rows.keys()])]
    .sort((left, right) => left - right);
  const chunks = [];
  const header = Buffer.alloc(8);
  header.writeInt32LE(indexes.length, 0);
  header.writeInt32LE(
    Math.max(source.declaredMax, translation.declaredMax, indexes.at(-1) ?? 0),
    4,
  );
  chunks.push(header);

  let translatedFields = 0;
  let fallbackFields = 0;

  for (const index of indexes) {
    const indexBuffer = Buffer.alloc(4);
    indexBuffer.writeInt32LE(index, 0);
    chunks.push(indexBuffer);

    const sourceFields = source.rows.get(index) ?? [];
    const translated = translation.rows.get(index) ?? [];

    for (let fieldIndex = 0; fieldIndex < stringCount; fieldIndex += 1) {
      let value = getTranslatedField(translated, fieldIndex, stringCount);
      if (isUsable(value)) {
        translatedFields += 1;
      } else {
        value = sourceFields[fieldIndex];
        if (!isUsable(value)) {
          value = Buffer.from(" ", "ascii");
        }
        fallbackFields += 1;
      }

      const length = Buffer.alloc(4);
      length.writeInt32LE(value.length, 0);
      chunks.push(length, value);
    }
  }

  return {
    buffer: Buffer.concat(chunks),
    rows: indexes.length,
    translatedFields,
    fallbackFields,
  };
}

function main(argv) {
  const [sourceArg, translationArg, outputArg, countArg] = argv;
  const stringCount = Number(countArg);
  if (!sourceArg || !translationArg || !outputArg || !Number.isInteger(stringCount) || stringCount < 1) {
    throw new Error(
      "Uso: node merge-localization-lod.js <base.lod> <traducción.lod> <salida.lod> <campos>.",
    );
  }

  const sourcePath = path.resolve(sourceArg);
  const translationPath = path.resolve(translationArg);
  const source = parseLod(sourcePath, stringCount);
  const translationCount = detectStringCount(translationPath, stringCount);
  const translation = parseLod(translationPath, translationCount);
  const result = mergeLod(source, translation, stringCount);
  fs.writeFileSync(path.resolve(outputArg), result.buffer);
  process.stdout.write(JSON.stringify({
    rows: result.rows,
    translationFieldsPerRow: translationCount,
    translatedFields: result.translatedFields,
    fallbackFields: result.fallbackFields,
  }));
}

try {
  main(process.argv.slice(2));
} catch (error) {
  process.stderr.write(`${error.message}\n`);
  process.exitCode = 1;
}
