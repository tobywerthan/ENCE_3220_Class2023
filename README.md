# **ENCE_3220_Class2023 Phase D**
Phase D of Embedded System tasked students to design their own PCB using the STM32f407G and the STM32CubeIDE. A friend of mine recently asked me if I could design a deivce that would help him monitor the temperature in his bearded dragon's enclosure. This project seemed like the perfect opportunity to begin designing such a device. 

## Project Requirements
The requirements for this temperature monitor aren't complex. The system must be able to read the current temperature, additional data is a plus. The system must also be able to display that data in a readable manner. The system must utilize the STM32F407G Discovery Board, and the software must be developed in the STM32CubeIDE. In summary, the system requirements are as follows: 
- Read temperature
- Display temperature in real time
- Utilize the STM32F407G Discovery Board
- Software developed in the STM32CubeIDE

## System Design
The design for this system includes the STM32F407G Discovery Board, an lcd display, and a temperature sensor. A block diagram of key components and their pin connections to the STM32F4. 

![BlockDiagram](https://github.com/tobywerthan/ENCE_3220_Class2023/assets/55803740/67a1513a-c1a0-4130-bfb1-48bb881af1c0)

## Components Selection
Key components for this system include the LCD1602A display, the DHT11 temperature and humidity sensor, and the STM32F4 discovery board. A full list of the required components can be seen in the table below. 

| References  | Value       | Footprint   |
| ----------- | ----------- | ----------- |
| U2 | DHT11 | Connector_PinSocket_2.54mm:PinSocket_1x03_P2.54mm_Vertical_SMD_Pin1Right |
| U1 | LCD1602A | Connector_PinSocket_2.54mm:PinSocket_1x16_P2.54mm_Vertical_SMD_Pin1Right |
| J1 & J2 | STM32F4 Discovery Board | 2x Connector_PinSocket_2.54mm:PinSocket_2x25_P2.54mm_Vertical |
| RV1 | 1000Ω | Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical_SMD_Pin1Right |
| R1 | 910Ω | Resistor_SMD:R_0603_1608Metric |
| C1 | 1mF | Capacitor_SMD:C_0603_1608Metric |

## Build Prototype


## STM32 Shield Design
The goal for this project was to develop a shield for the STM32F4 that implements the designed system. 

### Schematic
The schematic for this system contains three modules. The modules are categorized by component (ie. LCD display, DHT11, STM32F4). 

#### LCD Display

#### DHT11

#### STM32F4


### PCB

## Software Development


## Future Iterations
