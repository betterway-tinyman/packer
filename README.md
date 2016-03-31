Packer
=========
3D Bin Packing with multiple wrappers (boxes).

## References
##### Demo
![alt text](http://betterwaysystems.github.io/packer/demo/packer.gif "Packer")
  - Web: http://betterwaysystems.github.io/packer/demo/
  - Video: http://betterwaysystems.github.io/packer/demo/packer.mp4

##### Programming
- API Documents
  - C++ API Document: http://betterwaysystems.github.io/packer/api/cpp/
  - TypeScript API Document: http://betterwaysystems.github.io/packer/api/ts

##### Algorithm from Boxologic
  - Boxologic: https://github.com/exad/boxologic
  - Airforce Bin Packing: http://betterwaysystems.github.io/packer/reference/AirForceBinPacking.pdf

## Installation
##### TypeScript (JavaScript) only
If you want the TypeScript (JavaScript) only mode, any installation procedure is not required.
  - `release/ts/index.html`
  - http://betterwaysystems.github.io/packer/

##### Build Cloud Server
However, if you want to install the C++ mode, you've to install Visual C++ Redistributable for Visual Studio 2015. After the installation, execute `release/cpp/Packer.exe`. Then a cloud server deducting packer solution will be opened. After running the cloud server, open `release/ts/index.html`.

You also can separate cloud server(C++) and clients(Web), let users to connect remote Packer server, by editing ip address in  `release/ts/server.xml`

  - https://www.microsoft.com/en-US/download/details.aspx?id=48145
  - `release/cpp/Packer.exe`
  - `release/ts/index.html`
  - `release/ts/server.xml`

## Implementation
##### Design
![alt text](http://betterwaysystems.github.io/packer/design/class_diagram.png "Class Diagram")

##### Languages
- C++
- TypeScript
- Flex

##### Dependency
- C++
  - Samchon Framework (SDN Framework) - https://github.com/samchon/framework
    - Boost.Asio (C++ Socket Library) - http://www.boost.org/
- TypeScript (JavaScript)
  - STL for TypeScript (STL containers for TypeScript) - https://github.com/samchon/stl
  - Samchon Framework (SDN Framework) - https://github.com/samchon/framework
  - Three.js (JavaScript library handling 3-D objects) - http://threejs.org/
