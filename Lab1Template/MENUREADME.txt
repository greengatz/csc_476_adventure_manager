How to use the menu class:

By calling the setData function of Menu, you initialize all the data that Menu needs to draw a menu.

setData takes a string for the title, a vector of strings to describe what the menu is stating, and a vector of option structs.

The 'about string vector' will print each string in the vector on a new line.
The option struct has two elements: a string and a function pointer. The function being passed must follow this template:
	void funcTest()
The string element in the option struct describes an option that the player can choose from. The function pointed to by the function pointer is called when the player selects the option that corresponds with it.

As soon as setData is called, the menu will be displayed. The user can uses keys 1-5 to choose a different option to execute. The number of keys that can be used are limited to the number of options that are passed.

When a user makes a selection, the menu will close.

Here is an example:

	//Create about vector and add an element
  	vector<string> about;
	about.push_back("about test");

	//Create an option and add it to a vector
	option testOpt = {"test option", test};
	vector<option> options;
	options.push_back(testOpt);

	//Set the data
	menu.setData("Title", about, options);