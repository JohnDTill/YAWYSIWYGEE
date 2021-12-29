# YAWYSIWYGEE
This story arc has reached its conclusion, but the adventure continues with YAWYSIWYGEE 2: [Forscape](https://github.com/JohnDTill/Forscape)!
## Yet Another WYSIWYG Equation Editor
#### Yet Another What-you-see-is-what-you-get Equation Editor

Qt widget for editing documents with inline equations.

![alt text](demo.gif?raw=true "YAWYSIWGEE")

[Try it in your browser!](https://johndtill.github.io/YAWYSIWYGEE_WASM/)

## Purpose

This project aims to create a Qt widget for inline editing of equations. The target is a code editor style rather than a document editor style (think Notepad++ with equations versus MS Word or LyX). Such an equation code editor could be used for:

* Computer algebra systems
* Symbolic math
* Scientific computing languages
* Mathematical scratchpads
* Online use by compiling to WebAssembly
* The next hit mobile-game which is a cross between mathematics and Pictionary ("Gauss's law! No, wait, projectile motion!")

The name is perhaps overly self-deprecating; although there are many inline equation editors, there is not an abundance of open source options, and a Qt widget should be useful.

## Usage

There is an exposed widget class "TypesetEdit" which has an API similar to QTextEdit. Probably the easiest way to interact with a TypesetEdit in a separate project is to use the toMathBran() and setMathBran() methods, which use the [MathBran](https://github.com/JohnDTill/MathBran) serialization format. It is also possible to inject MathBran code into a specific part of the document, which is how the GUI buttons are implemented and how copy/paste works.

There is also an exposed class "TypesetToolbar" included in the library to make it easier to incorporate a GUI in a custom application. See the ExampleApp for a demonstration of how to use TypesetToolbar.

## Design

Discussion of data structures and algorithms can be found in the [project wiki](https://github.com/JohnDTill/YAWYSIWYGEE/wiki/Design). Hopefully these pages will lower the barrier of entry to modify YAWYSIWGEE source code.

## Prerequisites

* Qt (https://www.qt.io/download)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Contributing

Feel free to submit pull requests and use the issue tracker to start a discussion.

## Acknowledgements

Thanks to:
* Alexander Lange, developer of the Quivira font, which has exceptional unicode coverage and is generously donated to the public domain. (http://quivira-font.com/)
* My data structures and OO instructor Mark Boshart.
* Bob Nystrom, whose book CraftingInterpreters provides amazingly accessible lessons on parsing, among other language topics. (http://www.craftinginterpreters.com/)
* Creators of various unicode tools which were invaluable while coding this
  * https://www.unicodeit.net/ by Sven Kreiss and Kyle Cranmer.
  * https://www.branah.com/unicode-converter
  * Various excellent Wikipedia articles with tables
* The Qt Company.
