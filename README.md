# Uexküll For Daisy

This is a prototype for the Uexküll module using Daisy libraries and hardware. Eventual refactoring and porting to some other STM32 chip, plus refactoring to [CuteOps](https://github.com/cutelabnyc/cute-op) and PlatformIO

## Installation 🍤

Install that sweet ARM and dfu sauce

```
brew install make armmbed/formulae/arm-none-eabi-gcc dfu-util
```

Make the submodules 

```
cd DaisySP && make
cd ..
cd libDaisy && make
```
Make the hardware

```
make
make program-dfu
```

## Who Was Uexküll 🐡

Jakob Johann Freiherr von Uexküll was a Baltic German biologist who worked in the fields of muscular physiology, animal behaviour studies, and the cybernetics of life.

Uexküll was particularly interested in how living beings perceive their environment(s). He argued that organisms experience life in terms of species-specific, spatio-temporal, 'self-in-world' subjective reference frames that he called Umwelt (translated as surrounding-world, phenomenal world, self-world, environment - lit. German environment). These Umwelten (plural of Umwelt) are distinctive from what Uexküll termed the "Umgebung" which would be the living being's surroundings as seen from the likewise peculiar perspective or Umwelt of the human observer. Umwelt may thus be defined as the perceptual world in which an organism exists and acts as a subject. By studying how the senses of various organisms like ticks, sea urchins, amoebae, jellyfish and sea worms work, he was able to build theories of how they experience the world. Because all organisms perceive and react to sensory data as signs, Uexküll argued that they were to be considered as living subjects. This argument was the basis for his biological theory in which the characteristics of biological existence ("life") could not simply be described as a sum of its non-organic parts, but had to be described as subject and a part of a sign system.

*thanks wikipedia :-)*