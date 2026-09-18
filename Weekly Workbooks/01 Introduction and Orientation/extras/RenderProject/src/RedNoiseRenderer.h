#pragma once

#include "Renderer.h"

class RedNoiseRenderer: public Renderer {
   public:
      void draw(DrawingWindow &window) override;
};
