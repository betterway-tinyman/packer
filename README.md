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
  - http://betterwaysystems.github.io/packer/
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
	instanceArray->emplace(instanceArray->end(), 15, make_shared<Product>("Eraser", 1, 2, 5));
	instanceArray->emplace(instanceArray->end(), 15, make_shared<Product>("Book", 15, 30, 3));
	instanceArray->emplace(instanceArray->end(), 15, make_shared<Product>("Drink", 3, 3, 10));
	instanceArray->emplace(instanceArray->end(), 15, make_shared<Product>("Umbrella", 5, 5, 20));
	
	// Wrappers also can be packed into another Wrapper.
	instanceArray->emplace(instanceArray->end(), 15, make_shared<Wrapper>("Notebook-Box", 2000, 30, 40, 4, 2));
	instanceArray->emplace(instanceArray->end(), 15, make_shared<Wrapper>("Tablet-Box", 2500, 20, 28, 2, 0));
	
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
	cout << xml->toStrin() << endl;
	
	return 0;
}
```
