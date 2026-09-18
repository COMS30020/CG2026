#pragma once

#include "Renderer.h"

class ColourSpectrumRenderer: public Renderer {
   public:
      void draw(DrawingWindow &window) override;
};
