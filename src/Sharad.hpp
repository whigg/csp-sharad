////////////////////////////////////////////////////////////////////////////////////////////////////
//                               This file is part of CosmoScout VR                               //
//      and may be used under the terms of the MIT license. See the LICENSE file for details.     //
//                        Copyright: (c) 2019 German Aerospace Center (DLR)                       //
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CSP_SHARAD_HPP
#define CSP_SHARAD_HPP

#include "../../../src/cs-core/GraphicsEngine.hpp"
#include "../../../src/cs-scene/CelestialObject.hpp"

#include <VistaKernel/GraphicsManager/VistaOpenGLDraw.h>
#include <VistaKernel/GraphicsManager/VistaSceneGraph.h>
#include <VistaOGLExt/VistaBufferObject.h>
#include <VistaOGLExt/VistaGLSLShader.h>
#include <VistaOGLExt/VistaVertexArrayObject.h>

#include <memory>

class VistaTexture;

namespace csp::sharad {

/// Renders a single SHARAD image.
class Sharad : public cs::scene::CelestialObject, public IVistaOpenGLDraw {
 public:
  Sharad(std::shared_ptr<cs::core::GraphicsEngine> graphicsEngine, std::string const& sCenterName,
      std::string const& sFrameName, std::string const& sTiffFile, std::string const& sTabFile);

  Sharad(Sharad const& other) = delete;
  Sharad(Sharad&& other)      = delete;

  Sharad& operator=(Sharad const& other) = delete;
  Sharad& operator=(Sharad&& other) = delete;

  ~Sharad() override;

  void update(double tTime, cs::scene::CelestialObserver const& oObs) override;

  bool Do() override;
  bool GetBoundingBox(VistaBoundingBox& bb) override;

 private:
  class FramebufferCallback : public IVistaOpenGLDraw {
   public:
    FramebufferCallback(VistaTexture* pDepthBuffer);

    bool Do() override;
    bool GetBoundingBox(VistaBoundingBox& bb) override {
      return true;
    }

   private:
    VistaTexture* mDepthBuffer;
  };

  static std::unique_ptr<VistaTexture>        mDepthBuffer;
  static std::unique_ptr<FramebufferCallback> mPreCallback;
  static std::unique_ptr<VistaOpenGLNode>     mPreCallbackNode;
  static int                                  mInstanceCount;

  std::shared_ptr<cs::core::GraphicsEngine> mGraphicsEngine;
  std::unique_ptr<VistaTexture>             mTexture;

  VistaGLSLShader        mShader;
  VistaVertexArrayObject mVAO;
  VistaBufferObject      mVBO;

  int    mSamples;
  double mCurrTime   = -1.0;
  double mSceneScale = -1.0;

  static const char* VERT;
  static const char* FRAG;
};

} // namespace csp::sharad

#endif // CSP_SHARAD_HPP
