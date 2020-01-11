# Progvo-PM 0.9-skizo

Progvo-PM is a basic IDE for the Progvo programming language. It includes an Ecii editor and a Progvo interpreter.

It is written in C++ and uses SFML for the graphical interface.

![Hello, world!](https://progvo.dev/Bildoj/Progvo-PM_Saluton_Mondo.png)

## Compiling instructions

### On Debian/Ubuntu

You can compile Progvo-PM with g++ and make. Then, get [SFML](https://www.sfml-dev.org/), used for the graphical interface. You should also install Git to get the source code.

On Debian or Ubuntu, you can easily install these by doing as root:

```bash
apt install g++ make libsfml-dev git
```

Then, download the source files, go/`cd` to the directory, and do a simple make:

```bash
git clone https://github.com/Pttn/Progvo.git
cd Progvo/Progvo-PM
make
```

### In Windows x64

Untested.

You can compile Progvo-PM in Windows, and here is one way to do this. First, install [MSYS2](http://www.msys2.org/) (follow the instructions on the website), then enter in the MSYS **MinGW-w64** console, and install the tools and dependencies:

```bash
pacman -S make git
pacman -S mingw64/mingw-w64-x86_64-sfml
```

Clone the Progvo repository with `git`, go to the Progvo-PM directory with `cd`, and compile with `make`.

## Usage

Browse files with the right panel, type code in the text field.

Select a file in the browser and click on "Ŝargi" to load its content. Save a file by writing its name in the inline text field and clicking in "Konservi".

Click in "Plenumi" to interpret your code. Errors will be shown in the Progvo Console.

Progvo source code examples are provided.

### Special chars

For accentued Esperanto letters, use for example <kbd>Alt</kbd> + <kbd>C</kbd> to type `ĉ`. Other special characters:

* <kbd>Alt</kbd> + <kbd>!</kbd> → `¬`
* <kbd>&</kbd> → `∧`
* <kbd>Alt</kbd> + <kbd>&</kbd> → `∨`
* <kbd>Alt</kbd> + <kbd>*</kbd> → `×`
* <kbd>Alt</kbd> + <kbd>/</kbd> → `÷`
* <kbd>Alt</kbd> + <kbd>v</kbd> → `↓`
* <kbd>Alt</kbd> + <kbd>^</kbd> → `↑`
* <kbd>Alt</kbd> + <kbd><</kbd> → `←`
* <kbd>Alt</kbd> + <kbd>></kbd> → `→`
* <kbd>$</kbd> → `★`
* <kbd>Alt</kbd> + <kbd>$</kbd> → `§`

See also the [Ecii definition](https://progvo.dev/Ekio.html).