# Overview

## Glossary
- LTS = [Labelled Transition System](https://en.wikipedia.org/wiki/Transition_system), which is templated/monomorphized with its Key type and Transition type.

## Types

- The product recipe/process flow is formalised as a `LTS<KeyT = std::string, TransitionT = CompositeOperation>`
- A machine resource is formalised as a `LTS<KeyT = std::string, TransitionT = IOperation>`
- The topology, which is the result of merging/combining multiple Labelled Transition Systems is a `LTS<KeyT = std::vector<std::string>>, TransitionT = Operation>`