const { getMonospaceFonts } = require("../binding");
const assert = require("assert");
const os = require("os");

assert(getMonospaceFonts, "The expected function is undefined");

function shouldReturnListOfMonospaceFonts() {
  const fonts = getMonospaceFonts();

  console.log();
  console.log(fonts.join("\n"));
  console.log();
  assert.ok(fonts.length > 0, "No monospace fonts found");

  switch (os.platform()) {
    case "darwin":
      assert.ok(
        [
          "Andale Mono",
          "Courier",
          "Courier New",
          "Menlo",
          "Monaco",
          "PT Mono",
        ].every((font) => fonts.includes(font)),
        "Does not match the expected font list"
      );
      break;
    case "linux":
      assert.ok(
        ["DejaVu Sans Mono", "Liberation Mono"].every((font) =>
          fonts.includes(font)
        ),
        "Does not match the expected font list"
      );
      break;
    case "win32":
      assert.ok(
        ["Consolas", "Courier New", "Lucida Console"].every((font) =>
          fonts.includes(font)
        ),
        "Does not match the expected font list"
      );
      break;
  }
}

assert.doesNotThrow(
  shouldReturnListOfMonospaceFonts,
  undefined,
  "shouldReturnListOfMonospaceFonts threw an expection"
);

console.log("All tests passed.");
