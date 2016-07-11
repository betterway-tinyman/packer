﻿Packer
=========
3D Bin Packing with multiple wrappers (boxes).

##### Red Printing
Red-Printing is a brand name of printing service supported by Betterway-systems.

레드 프린팅은 베러웨이시스템즈에서 실시하는 인쇄 서비스의 브랜드 명입니다. 많은 이용 바랍니다. 

Packer를 이용하시는 여러분, 인쇄 주문은 꼭 레드프린팅에서 해 주세요.

http://redprinting.co.kr/

##### Author
  - Jeongho Nam <http://samchon.org/>
  - developer in Betterway-systems <http://redprinting.co.kr/>

## References
##### Demo
<<<<<<< HEAD
![Packer Demo GIF](http://betterwaysystems.github.io/packer/demo/video_small.gif "Packer Demo GIF")
=======
<img src="http://betterwaysystems.github.io/packer/demo/video_small.gif">
>>>>>>> 62c12dab33cdea6f73ce66cb8548d6b9ab936594

  - Web: http://betterwaysystems.github.io/packer/demo
  - Video: http://betterwaysystems.github.io/packer/demo/packer.mp4

##### Programming
- API Documents
	- C++ API Document: http://betterwaysystems.github.io/packer/api/cpp/
	- TypeScript API Document: http://betterwaysystems.github.io/packer/api/ts

##### Algorithm from Airforce Bin Packing
  - Thesis - [The distributer's three-midemsional pallet-packing problem: A human intelligence-based heuristic approach](http://betterwaysystems.github.io/packer/reference/AirForceBinPacking.pdf)
  - GitHub: https://github.com/exad/boxologic

## Installation
##### TypeScript (JavaScript) only
If you want the TypeScript (JavaScript) only mode, any installation procedure is not required.
  - http://betterwaysystems.github.io/packer/demo/
  - `release/ts/index.html`

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
  - Server solving packing problem.
  - Deduct the best optimization result with genetic algorithm
- TypeScript
  1. Act a role of client connecting to C++ server.
  2. Do packing itself without C++ server and do not use genetic algorithm.
    - The optimization result can be inferior than C++
- Flex
  - I'm not familiar with HTML5 
  - To realize item editor faster, I've adopted the Flex.
  - However, it will be replaced to the HTML5 soon.

##### Dependency
- C++
	- Samchon Framework (SDN Framework) - https://github.com/samchon/framework
		- Boost.Asio (C++ Socket Library) - http://www.boost.org/
- TypeScript (JavaScript)
	- STL for TypeScript (STL containers for TypeScript) - https://github.com/samchon/stl
	- Samchon Framework (SDN Framework) - https://github.com/samchon/framework
	- Three.js (JavaScript library handling 3-D objects) - http://threejs.org
	- React - https://facebook.github.io/react
	- React-Data-Grid - https://github.com/adazzle/react-data-grid

## Usage
##### TypeScript (JavaScript)
```typescript
function main(): void
{
	///////////////////////////
	// CONSTRUCT OBJECTS
	///////////////////////////
	let wrapperArray: bws.packer.WrapperArray = new bws.packer.WrapperArray();
	let instanceArray: bws.packer.InstanceArray = new bws.packer.InstanceArray();
	
	// Wrappers
	wrapperArray.push
	(
		new bws.packer.Wrapper("Large", 1000, 40, 40, 15, 0),
		new bws.packer.Wrapper("Medium", 700, 20, 20, 10, 0),
		new bws.packer.Wrapper("Small", 500, 15, 15, 8, 0)
	);
	
	///////
	// Each Instance is repeated #15
	///////
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Eraser", 1, 2, 5));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Book", 15, 30, 3));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Drink", 3, 3, 10));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Product("Umbrella", 5, 5, 20));
	
	// Wrappers also can be packed into another Wrapper.
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Wrapper("Notebook-Box", 2000, 30, 40, 4, 2));
	instanceArray.insert(instanceArray.end(), 15, new bws.packer.Wrapper("Tablet-Box", 2500, 20, 28, 2, 0));
	
	///////////////////////////
	// BEGINS PACKING
	///////////////////////////
	// CONSTRUCT PACKER
	let packer: bws.packer.Packer = new bws.packer.Packer(wrapperArray, instanceArray);
	
	///////
	// PACK (OPTIMIZE)
	let result: bws.packer.WrapperArray = packer.optimize();
	///////
	
	///////////////////////////
	// TRACE PACKING RESULT
	///////////////////////////
	let xml: samchon.library.XML = result.toXML();
	samchon.trace(xml);
}
```

##### C++
```cpp
#include <iostream>
#include <bws/packer/Packer.hpp>

#include <bws/packer/WrapperArray.hpp>
#include <bws/packer/InstanceArray.hpp>
# include <bws/packer/Product.hpp>
# include <bws/packer/Wrapper.hpp>

using namespace std;
using namespace samchon::library;
using namespace bws::packer;

int main()
{
	///////////////////////////
	// CONSTRUCT OBJECTS
	///////////////////////////
	shared_ptr<WrapperArray> wrapperArray(new WrapperArray());
	shared_ptr<InstanceArray> instanceArray(new InstanceArray());
	
	// Wrappers
	wrapperArray->emplace_back(new bws.packer.Wrapper("Large", 1000, 40, 40, 15, 0));
	wrapperArray->emplace_back(new Wrapper("Medium", 700, 20, 20, 10, 0));
	wrapperArray->emplace_back(new Wrapper("Small", 500, 15, 15, 8, 0));
	
	///////
	// Each Instance is repeated #15
	///////
	instanceArray->insert(instanceArray->end(), 15, make_shared<Product>("Eraser", 1, 2, 5));
	instanceArray->insert(instanceArray->end(), 15, make_shared<Product>("Book", 15, 30, 3));
	instanceArray->insert(instanceArray->end(), 15, make_shared<Product>("Drink", 3, 3, 10));
	instanceArray->insert(instanceArray->end(), 15, make_shared<Product>("Umbrella", 5, 5, 20));
	
	// Wrappers also can be packed into another Wrapper.
	instanceArray->insert(instanceArray->end(), 15, make_shared<Wrapper>("Notebook-Box", 2000, 30, 40, 4, 2));
	instanceArray->insert(instanceArray->end(), 15, make_shared<Wrapper>("Tablet-Box", 2500, 20, 28, 2, 0));
	
	///////////////////////////
	// BEGINS PACKING
	///////////////////////////
	// CONSTRUCT PACKER
	Packer packer(wrapperArray, instanceArray);
	GAParameters gaParams = {500, 100, 50, 0.2};
	
	///////
	// PACK (OPTIMIZE)
	///////
	shared_ptr<WrapperArray> &result = packer.optimize(gaParams);
	
	///////////////////////////
	// TRACE PACKING RESULT
	///////////////////////////
	shared_ptr<XML> xml = result->toXML();
	cout << xml->toString() << endl;
	
	return 0;
}
```

## License
##### BSD v3.

Copyright (c) 2016, betterwaysystems
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of packer nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
