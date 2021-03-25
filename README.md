# node-monospace-fonts

> Get a list of monospace fonts installed on your system. Prebuilt N-API module created for [shifty](https://marketplace.visualstudio.com/items?itemName=bmealhouse.shifty) automatic font detection. Based on code from [font-manager](https://github.com/foliojs/font-manager).

## Table of contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Installation

### yarn

```sh
yarn add node-monospace-fonts
```

### npm

```sh
npm i node-monospace-fonts --save
```

## Usage

```js
import { getMonospaceFonts } from "node-monospace-fonts";

// Logs monospace fonts installed on your system
console.log(getMonospaceFonts());

// [
//   "Andale Mono",
//   "Courier",
//   "Courier New",
//   "Menlo",
//   "Monaco",
//   "PT Mono"
// ]
```

## Contributing

TODO

## License

MIT
