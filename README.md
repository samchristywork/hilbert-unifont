![Banner](https://s-christy.com/status-banner-service/hilbert-unifont/banner-slim.svg)

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

This is the Hilbert Curve, it is rendered here as a hue gradient, and starts in
the upper left hand corner and ends in the upper right.

![The Hilbert Curve](https://github.com/samchristywork/hilbert-unifont/blob/main/examples/hilbert_curve.png)

This is Unifont arranged in a traditional left-to-right, top-to-bottom fashion.
This arrangement causes artifacts like horizontal banding.

![Unifont in a linear arrangement](https://github.com/samchristywork/hilbert-unifont/blob/main/examples/linear_unifont.png)

This is Unifont arranged along the Hilbert Curve. This arrangement groups
together similar groups of characters. This is especially useful for groups that
are divided on powers of two as Unicode's Plane 0 is.

![Unifont arranged on the Hilbert Curve](https://github.com/samchristywork/hilbert-unifont/blob/main/examples/hilbert_unifont.png)

## Credits

Information about the Hilbert Curve, including the algorithm for translating
between one- and two-dimensional representations of the curve were found in the
[Wikipedia article for the Hilbert Curve](https://en.wikipedia.org/wiki/Hilbert_curve).

Code that was used for cycling through UTF codepoints was based on
[this Stack Overflow post](https://stackoverflow.com/questions/38491380/how-to-print-unicode-codepoints-as-characters-in-c).

Code for converting between RGB and HSV representations of color was based on
[this file on GitHub](https://github.com/Inseckto/HSV-to-RGB/blob/master/HSV2RGB.c).

## License

This work is licensed under the GNU General Public License version 3 (GPLv3).

[<img src="https://s-christy.com/status-banner-service/GPLv3_Logo.svg" width="150" />](https://www.gnu.org/licenses/gpl-3.0.en.html)
