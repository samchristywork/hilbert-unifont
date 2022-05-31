## Overview

The Hilbert Curve is a space-filling curve that preserves the locality of
elements in a sequence. In other words, elements that are close together in a
one-dimensional sequence, are also close together on the 2d plane when arranged
along the Hilbert Curve.

Unifont is a font that has very thorough coverage of the basic multilingual
plane. I arranged the glyphs of this font on a 2d plane using both reading
order (left to right and top to bottom) and using the Hilbert Curve. I think
the Curve has properties that make it a better viewing experience than the
alternative.

## Dependencies

```
gcc
libcairo2-dev
make
```

## Examples

![The Hilbert Curve](https://github.com/samchristywork/hilbert-unifont/blob/main/examples/hilbert_curve.png)

![Unifont in a linear arrangement](https://github.com/samchristywork/hilbert-unifont/blob/main/examples/linear_unifont.png)

![Unifont arranged on the Hilbert Curve](https://github.com/samchristywork/hilbert-unifont/blob/main/examples/hilbert_unifont.png)
