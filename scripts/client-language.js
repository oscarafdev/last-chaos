"use strict";

const fs = require("fs");
const path = require("path");

const SEED = Buffer.from(';!k.az"MAEjhgasbube18340-fZ,;asAOJM.joqwAsefFsFjd', "ascii");
const COUNTRY_PATTERN = /persistent extern INDEX g_iCountry=\(INDEX\)(\d+);/;
const SAVE_ID_PATTERN = /persistent extern INDEX g_iSaveID=\(INDEX\)\d+;/;
const SAVED_USERNAME_PATTERN = /persistent extern user CTString g_strSaveID="[^"]*";/;

const languages = Object.freeze({
  de: { country: 10, name: "Alemán" },
  es: { country: 11, name: "Español" },
  fr: { country: 12, name: "Francés" },
  it: { country: 17, name: "Italiano" },
  pl: { country: 13, name: "Polaco" },
  ru: { country: 14, name: "Ruso" },
  uk: { country: 24, name: "Inglés (UK)" },
  us: { country: 7, name: "Inglés (USA)" },
});

function transformPersistentSymbols(source, decode) {
  const result = Buffer.alloc(source.length);

  for (let index = 0; index < source.length; index += 1) {
    const seedByte = SEED[(source.length + (23 * index)) % SEED.length];

    if (decode) {
      const shift = 8 - (((source.length - index) % 8) + 1);
      const value = source[index] ^ seedByte;
      const shifted = (value << 16) << shift;
      const swapped = (shifted & 0x00ff0000) | ((shifted & 0xff000000) >>> 8);
      result[index] = (swapped >>> 16) & 0xff;
    } else {
      const shift = ((source.length - index) % 8) + 1;
      let value = source[index] << shift;
      value = (value & 0xff) | ((value & 0xff00) >>> 8);
      result[index] = (value & 0xff) ^ seedByte;
    }
  }

  return result;
}

function decodeFile(file) {
  if (file.length < 4) {
    throw new Error("ps.dat no contiene la cabecera de longitud.");
  }

  const length = file.readInt32LE(0);
  if (length < 1 || file.length < length + 4) {
    throw new Error(`Longitud inválida en ps.dat: ${length}.`);
  }

  return transformPersistentSymbols(file.subarray(4, 4 + length), true);
}

function encodeFile(decoded) {
  const header = Buffer.alloc(4);
  header.writeInt32LE(decoded.length, 0);
  return Buffer.concat([header, transformPersistentSymbols(decoded, false)]);
}

function readCountry(filePath) {
  const text = decodeFile(fs.readFileSync(filePath)).toString("latin1");
  const match = text.match(COUNTRY_PATTERN);
  if (!match) {
    throw new Error("No se encontró g_iCountry en ps.dat.");
  }
  return Number(match[1]);
}

function setCountry(filePath, country) {
  const decoded = decodeFile(fs.readFileSync(filePath));
  const text = decoded.toString("latin1");
  if (!COUNTRY_PATTERN.test(text)) {
    throw new Error("No se encontró g_iCountry en ps.dat.");
  }

  const updated = Buffer.from(
    text.replace(COUNTRY_PATTERN, `persistent extern INDEX g_iCountry=(INDEX)${country};`),
    "latin1",
  );
  fs.writeFileSync(filePath, encodeFile(updated));
}

function sanitizeSavedAccount(filePath) {
  const decoded = decodeFile(fs.readFileSync(filePath));
  const text = decoded.toString("latin1");
  if (!SAVE_ID_PATTERN.test(text) || !SAVED_USERNAME_PATTERN.test(text)) {
    throw new Error("No se encontraron los campos de cuenta guardada en ps.dat.");
  }

  const updated = Buffer.from(
    text
      .replace(SAVE_ID_PATTERN, "persistent extern INDEX g_iSaveID=(INDEX)0;")
      .replace(SAVED_USERNAME_PATTERN, 'persistent extern user CTString g_strSaveID="";'),
    "latin1",
  );
  fs.writeFileSync(filePath, encodeFile(updated));
}

function readSavedAccount(filePath) {
  const text = decodeFile(fs.readFileSync(filePath)).toString("latin1");
  const saveID = text.match(SAVE_ID_PATTERN)?.[0].match(/\d+/)?.[0];
  const username = text.match(SAVED_USERNAME_PATTERN)?.[0].match(/"([^"]*)"/)?.[1];
  if (saveID === undefined || username === undefined) {
    throw new Error("No se encontraron los campos de cuenta guardada en ps.dat.");
  }
  return { saveID: Number(saveID), username };
}

function main(argv) {
  const [command, fileArg, languageCode] = argv;
  if (!command || !fileArg || !["get", "set", "sanitize", "account"].includes(command)) {
    throw new Error("Uso: node client-language.js <get|set|sanitize|account> <ps.dat> [idioma]");
  }

  const filePath = path.resolve(fileArg);
  if (command === "account") {
    const account = readSavedAccount(filePath);
    process.stdout.write(JSON.stringify(account));
    return;
  }

  if (command === "sanitize") {
    sanitizeSavedAccount(filePath);
    process.stdout.write("Cuenta guardada eliminada");
    return;
  }

  if (command === "get") {
    const country = readCountry(filePath);
    const language = Object.entries(languages).find(([, value]) => value.country === country);
    process.stdout.write(language ? `${language[0]} (${language[1].name})` : `country=${country}`);
    return;
  }

  const language = languages[languageCode?.toLowerCase()];
  if (!language) {
    throw new Error(`Idioma inválido. Valores admitidos: ${Object.keys(languages).join(", ")}.`);
  }
  setCountry(filePath, language.country);
  process.stdout.write(`${languageCode.toLowerCase()} (${language.name})`);
}

try {
  main(process.argv.slice(2));
} catch (error) {
  process.stderr.write(`${error.message}\n`);
  process.exitCode = 1;
}
