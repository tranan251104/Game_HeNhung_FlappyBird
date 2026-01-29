<br/>
<p align="center">
  <a href="https://github.com/ihbkaiser/ihbkaiser-Game-HeNhung20251">
    <img src="https://m.media-amazon.com/images/I/21ZzpgClpiL.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">FLAPPY BIRD GAME PROJECT</h3>

  <p align="center">
    Making flappybird game on STM32F429IZIT6-DISC0 card with TouchGFX
    <br/>
    <br/>
    
  
  </p>
</p>

<p align="center">
  
 ![Contributors](https://img.shields.io/github/contributors/ihbkaiser/ihbkaiser-Game-HeNhung20251?color=dark-green) ![Stargazers](https://img.shields.io/github/stars/ihbkaiser/ihbkaiser-Game-HeNhung20251?style=social) ![Issues](https://img.shields.io/github/issues/ihbkaiser/ihbkaiser-Game-HeNhung20251) 

</p>

## Table Of Contents

* [About the Project](#about-the-project)
* [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [Authors](#authors)
* [Acknowledgements](#acknowledgements)

## About The Project

![Screen Shot](Demo.jpg)

### Course Information

- Embedded Systems Assignment (Semester 20251) - HUST
- Topic: Flappy Bird Game

### Game Logic

- Main menu: tap the Flappy Bird icon tile to start the game.
- In-game: press the button to make the bird jump.
- Game over: tap the **BACK** button at the bottom to return to the main menu.
- Score increases when the bird passes a pipe column.

## Built With

The technologies I used during the project are as follows:

* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
* [TouchGFX](https://www.st.com/en/development-tools/touchgfxdesigner.html)

## Getting Started

I will explain how to load the project onto your own board here.

### Prerequisites

You should have STM32CubeIDE and TouchGFX installed on your computer.
The links have been provided in the [Built With](#built-with) section.

### Installation

Clone the repo

```sh
git clone https://github.com/ihbkaiser/Game-HeNhung20251.git
```



## Usage

After downloading the project to your computer, navigate to the 'STM32CubeIDE' folder inside the project folder and open the '.cproject' file with STM32CubeIDE. Once you have opened the project in the IDE, you can load it onto your STM32F429IZIT6-DISC0 board through the IDE.

### How to Run (Quick Steps)

1. Open `STM32CubeIDE/.cproject` in STM32CubeIDE.
2. Build the project (Debug configuration is fine).
3. Connect the STM32F429I-DISCO board via ST-LINK USB.
4. Click **Debug** or **Run** to flash and start.

## Roadmap

See the [open issues](https://github.com/ihbkaiser/ihbkaiser-Game-HeNhung20251/issues) for a list of proposed features (and known issues).

## Contributing

* If you have suggestions for adding or removing projects, feel free to [open an issue](https://github.com/ihbkaiser/ihbkaiser-Game-HeNhung20251/issues/new) to discuss it.
* Please make sure you check your spelling and grammar.
* Create individual PR for each suggestion.

### Creating A Pull Request

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

