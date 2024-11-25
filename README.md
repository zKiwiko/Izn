# Izn: Initialization Files

## Overview

Izn is a syntax format designed for configuration files, intended to enhance the functionality of traditional `.ini` files. It offers a more streamlined syntax that improves readability.  
These files serve as a demonstration of elegance in this category of file formats.

There are no limitations on the file extensions used; they can be `.izn`, `.txt`, or even `.ini`. Izn provides a unified syntax structure for configuration files, eliminating the necessity for different file types across various platforms.

---

## Ionize Parser (`Izn::Parser`) Docs
The `Izn::Parser` class is a simple library for parsing Izn Syntax-based configuration files.
It supports key-value pairs grouped into sections.

### Syntax Example
```less
@General:
  name: "Example"
  version: 1.00
  readmeversion: 1.00

@Logging:
  type: "debug"
  use_izn: true
  file_path: "data/log/file.log"
  max_size: 10
```
* Sections: Denoted by `@SectionName:`
* Keys and values wrriten as `key: value`.

### Features
Access by Section and Key: Retrieve values using the section and key names.
Default Fallback: If a key is missing, returns a default-constructed value for the requested type.

## Installation
Include the header file `izn.hpp` into your project.
```cpp
#include "izn.hpp"
```

## Usage
1> Parsing a Izn based file
```cpp
#include "izn.hpp"

int main() {

Izn::Parser parser("file.izn");

return 0;
}
```
2> Retrieving Values
```cpp

// By Type
std::string name = parser.get<std::string>("General", "name");
int version = parser.get<int>("General", "version");
bool use_izn = parser.get<bool>("Logging", "use_izn");

// Generic Value Retrieval
auto value = parser.getValue("General", "readmeversion");
std::visit([](const auto& val) { std::cout << val << std::endl; }, value);

```
3> Debugging Parsed Data
```cpp
parser.print();
```

## Extras
* Comments: Lines starting with `#` are ignored.
* Empty lines are obviously ignored.
* Error Handling: Logs warnings to `std::cerr` for type mismatches or missing keys.
* Default return values: For missing keys, `get` returns the default constructed value of the requested type.


