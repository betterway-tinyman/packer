# Samchon Framework
[![NPM](https://nodei.co/npm/samchon-framework.png?downloads=true&downloadRank=true&stars=true)](https://nodei.co/npm/samchon-framework)

GitHub Repository: https://github.com/samchon/framework


## What Samchon Framework is
### Object Oriented Network
![Network Connectivity to Class Relationship](http://samchon.github.io/framework/images/accessory/oon.png)

Samchon Framework is an OON (Object-Oriented Network) framework. You can compose network systems within framework of the Object-Oriented. 

Whether how the network system is enormous and complicate, **the system nodes, they're just objects** in the Samchon Framework. Define <u>objects who represent the *system nodes*</u> and <u>deliver message by calling methods from objects to objects</u>. That's all.

### System Templates, Modules of OON
I repeat, Samchon is an OON (Object-Oriented Network) framework, composing a network system within framework of the Object-Oriented. It means that even **modulization** is possible and it's called **System Templates**. You can maximize the **reusability** with the **modulization, System Templates**.

When you need a cloud system, then take a System Template named [Cloud Service](https://github.com/samchon/framework/wiki/TypeScript-Templates-Cloud_Service). When you need a distributed processings system, then take the [Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed-System). If you should implement a specific network system and the network system will be used repeatedly, then **modulize** it to be one of the **System Templates**.

Below **System Templates** are basically provided in Samcho Framework:

  - [Cloud Service](https://github.com/samchon/framework/wiki/TypeScript-Templates-Cloud_Service): Cloud Server Template
  - [External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External-System): A template for 1: N composite relationship's network system.
    - [Parallel System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Parallel-System): Parallel Processing System Template.
    - [Distributed System](https://github.com/samchon/framework/wiki/TypeScript-Templates-Distributed-System): Distributed Processing System Template.

### [TypeScript-STL](https://github.com/samchon/typescript-stl), interaction with TypeScript and C\++
[![Supported Languages](http://samchon.github.io/framework/images/accessory/language_diagram.png)](#interaction)

The best solution, for integrating and migrating a system built by different languages (TypeScript and C++), is to sharing same designs and grammers. Those designs and grammers are unitifed, then any other solution can't be better. Samchon Framework provides the best solution.

Unifying designs, it can be come true by utilizing the **Object-Oriented Network** paradigm with Samchon Framework. Unifying grammer, it also can be done by using the [**TypeScript-STL**](https://github.com/samchon/typescript-stl), a sub-project who migrated C\++'s STL (Standard Template Library) to TypeScript. You can use STL's containers and algorithms in TypeScript with the [**TypeScript-STL**](https://github.com/samchon/typescript-stl). Thus, you can unify not only designs but also grammers, algorithms and even data structures with the [**TypeScript-STL**](https://github.com/samchon/typescript-stl).

#### Suggestion
If you're planning to build a cloud solution, then I sugguest you to follow such step:
  1. Fast Implementation
    1. Cloud Application with TypeScript & HTML 
    2. Cloud Service with TypeScripty & NodeJS
  2. Performance Improvement
    1. Heavy processes, distribute them to C\++ systems.
    2. Migrate Cloud server from TypeScript to C++ gradually.


## Installation
#### TypeScript
```bash
# Install NPM module.
npm install -g samchon-framework

# Install Definition (*.d.ts) files.
npm install --save @types/samchon-framework
```

Installs Samchon-Framework from NPM module and imports the definition.

```typescript
/// <reference types="samchon-framework" />
import samchon = require("samchon-framework");
```

#### C++
Just include header files what you want.

```cpp
#include <samchon/lilbrary.hpp>
#include <samchon/protocol.hpp>

#include <samchon/templates/service.hpp>
#include <samchon/templates/external.hpp>
#	include <samchon/templates/parallel.hpp>
#	include <samchon/templates/distributed.hpp>
```


## Documents
#### Architecture Designs
  - Class Diagrams
    - [TypeScript](http://samchon.github.io/framework/design/ts_class_diagram.pdf)
    - [C++](http://samchon.github.io/framework/design/cpp_class_diagram.pdf)
  - [Conceptual Diagram](http://samchon.github.io/framework/design/conceptual_diagram.pdf)
  - [Sequence Diagram](http://samchon.github.io/framework/design/sequence_diagram.pdf)

#### Guide Documents
  - [TypeScript](https://github.com/samchon/framework/wiki/TypeScript-Protocol-Basic_Components)
  - [C++](https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components)
  - [Examples](https://github.com/samchon/framework/wiki/Examples-Interaction)
  
#### API Documents
  - **v2.0**
    - [**TypeScript**](http://samchon.github.io/framework/api/ts)
    - [__C++__](http://samchon.github.io/framework/api/cpp)
  - v1.0
    - [C++](http://samchon.github.io/framework/api/v1.0/cpp)
    - [Flex](http://samchon.github.io/framework/api/v1.0/flex)
