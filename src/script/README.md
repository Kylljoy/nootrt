# nootscript

Okay, so I have been at this for about three months now, trying to figure out how to allow a complete, customizable way of providing flexibility within this system. My prevailing theory was to allow customization through an excel-like chain of functions that could be restructured to allow the creator to plug-and-play different algorithms for dice-rolling, point allocating, combat, etc. But that proved too much of a headache. So I'm doing the simpler option.

A complete scripting language that compiles into bytecode. Hence, "nootscript". Nootscript programs will be basic pipeline programs that essentially run like a Turing machine: you give it a "stack" of entity structures/integers/strings/etc, and it outputs a "stack" of results. You can feasibly chain these programs together to create more complex interactions, and embed these programs into sheets or boards.
