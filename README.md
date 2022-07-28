# {PHG}
{PHG} : A mini programming language for describing everything.

![phg](https://user-images.githubusercontent.com/8099625/169991127-eddeb6bd-b67b-4359-a68a-14e16da3863d.png)
# PHG language parser structure design
![mainimg](./struct.png)
* ## kernel (base)
The PHG kernel is divided into three layers: basic grammar layer (PHG parser), node grammar layer (NODE parser), element and operation layer (element/nodecalc)
* ### Basic syntax layer
The basic syntax of PHG as a minimalist programming language is implemented in this layer. Specifically, it includes variable definitions, control statements, and function calls.
This language belongs to the interpretive language, which directly interprets and runs according to the given string code. The custom function is defined in the position of the string code,
Global custom functions are not currently supported, as follows:
* code code structure
* Internal definition ptr points to the current running position of the string code.
* Member functions are used for reading, shifting and other operations when executing code.
* Internal oprstack valstack strstack each stack, used for stack parameter storage during code execution
* Internal funcnamemap is used for function code location calibration
* rank is an array of operator rank settings
* gvarmapstack variable stack
* dostring() directly interprets and runs this string
* Internal check for syntax errors, including Chinese character check, brace matching check.
* dofile() run this code file
* doexpr() run this string by expression
* register_api() register API
* If there is a syntax error, the code execution is interrupted, and the overall crash caused by the syntax error is avoided as much as possible.
* ### Node syntax layer
In order to describe 3D scenes, a descriptive syntax is designed, drawing on syntax features such as JSON. The concrete realization is realized by constructing the tree data object.
The tree object (tree) has functions such as traversal and query, as well as operations such as copying and copying, and is encapsulated into an API for external use:
* Nodes have attribute expressions in the form of (a.p), which can be accessed directly to modify the attribute values ​​of nodes.
* im(),on() basic node, attribute access
* wak() traverses the tree structure and can execute statements node by node
* doexpr() runs the expression in the attribute and stores the final result as an attribute string
* dump() prints the entire tree
* ### Element and operation layer
* Define an operable class object (var_t) that overloads operators to implement variables and operations defined in the script. This variable class object has
The three data types: int, float, and string are distinguished by type tags. In addition, there is a resource ID, the user can pass this ID
To index the resource data defined by yourself, such as Boolean operations, 3D objects, etc. have their own defined resource data (mesh, transform, etc.). in addition
There are function pointers in the class object (var_t) that can point to the operation functions defined by the external resource class, including: four arithmetic operations, set() and so on.
* Various operations are implemented in the function _act().
* The operation of the node is in another module (nodecalc), and the node, that is, the tree object (tree), has its own defined operation logic, which can realize some intelligent operations.
* Addition operation The result of adding two nodes is their nearest common parent node.
* Subtraction Subtracts the second node of its children from the first node.
* ## instantiate the extension (entity)
The extension of PHG is divided into two ways: element overload extension and custom instance extension:
* ### element overload extension
* element can define an element class derived from var_t or varbase_t class, and then overload the operation. This method is used in scenarios such as Boolean operations, skipping the node layer and directly deriving from the base syntax layer.
* ### Custom instance extension
* Use the resource ID method to expand the element object. You can define a resource class at will, associate it with var_t through the resource ID, or use its function pointer.
		Specifically: 
* Define a resource list (reslist), a res(var), through var.resid, query the resource list and return a resource class pointer.
* setup() is used to traverse the tree structure, calculate the variables (such as transform) that need to be calculated in the resource class, and prepare before drawing (if it needs to be drawn).

* ## External use and interaction
PHG supports the parsing and exporting of json, xml and other formats, and there are special parsing classes to implement them. Support http communication.
There are several ways to use PHG:
* ### Code direct integration
* It can use a single model or encapsulate it as a class or structure, and the instances can be resolved independently or share some data.
* Encapsulates an interface module, which exposes limited interfaces internally.
* If you need to access internal objects such as node objects, you can include the implementation code into the PHG instantiation module. (Refer to the code implementation of the cabinet project for details)
* ### DLL can be called across languages
* You can use the dynamic link library to call the phg interface
* Can allow other programming languages ​​such as VB, C#, etc. to call the PHG interface and obtain data
* ### HTTP server-style usage
* The use of http server can support web pages JS, PYTHON, UNITY, VB and other clients, and obtain the result data by returning json, which is convenient for clients to use

* ## Code file structure
*-PHG
* -base
* +--element.hpp #Element class and operation overloading
* +--node.hpp #Node parser
* +--phg.hpp #phg basic parser
* +--phg_head.hpp
* -entity
* +--entity.hpp #3D entity implementation
* +--nodecalc.hpp #Node logic operation implementation
* +--sprite.hpp #Interface 2D sprite implementation
* -net
* +--httplib.hpp # http library
* +--server.cc # http server
* -parsers
* +--jsonparser.hpp # json parser
* +--xmlparser.hpp # xml parser
* +--scene.hpp # Entry class
# Sample:
![FV67FCfUEAEQQfb](https://user-images.githubusercontent.com/8099625/175246104-a1f453da-c92c-4afa-8660-b7fdfe00391a.png)

# API:
echo()  
dump()  
msgbox()  

rnd()  
sin()  
cos()  

im()  
on()  

wak()  
expr()  

add()  
sub()  
calc()  

draw()  
setup()  
tojson()  
