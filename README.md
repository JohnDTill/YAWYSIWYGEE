# YAWYSIWYGEE
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
* Online forums by compiling to WebAssembly

The name is perhaps overly self-deprecating; although there are many inline equation editors, there is not an abundance of open source options, and a Qt widget should be useful.

YAWYSIWYGEE uses the [MathBran](https://github.com/JohnDTill/MathBran) specification for serialization. Probably the easiest way to interact with a Typeset::Edit in a separate project is to use the toMathBran() and setMathBran() methods. It is also possible to inject MathBran code into a specific part of the document, which is how the example app GUI is implemented and how copy/paste works.

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
* The Qt Company.
