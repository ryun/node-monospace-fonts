const { getMonospaceFonts } = require("../binding");
const assert = require("assert");

assert(getMonospaceFonts, "The expected function is undefined");

function shouldReturnListOfMonospaceFonts() {
  const fonts = getMonospaceFonts();
  console.log();
  console.log(fonts.join("\n"));
  console.log();
  assert.ok(fonts.length > 0, "No monospace fonts found");
}

assert.doesNotThrow(
  shouldReturnListOfMonospaceFonts,
  undefined,
  "shouldReturnListOfMonospaceFonts threw an expection"
);

console.log("All tests passed.");
