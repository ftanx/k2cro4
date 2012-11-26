
/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#include "GrStencilAndCoverPathRenderer.h"
#include "GrContext.h"
#include "GrGpu.h"
#include "GrPath.h"

GrPathRenderer* GrStencilAndCoverPathRenderer::Create(GrContext* context) {
    GrAssert(NULL != context);
    GrAssert(NULL != context->getGpu());
    if (context->getGpu()->getCaps().pathStencilingSupport()) {
        return SkNEW_ARGS(GrStencilAndCoverPathRenderer, (context->getGpu()));
    } else {
        return NULL;
    }
}

GrStencilAndCoverPathRenderer::GrStencilAndCoverPathRenderer(GrGpu* gpu) {
    GrAssert(gpu->getCaps().pathStencilingSupport());
    fGpu = gpu;
    gpu->ref();
}

GrStencilAndCoverPathRenderer::~GrStencilAndCoverPathRenderer() {
    fGpu->unref();
}

bool GrStencilAndCoverPathRenderer::canDrawPath(const SkPath& path,
                                                GrPathFill fill,
                                                const GrDrawTarget* target,
                                                bool antiAlias) const {
    return kHairLine_GrPathFill != fill &&
           !antiAlias && // doesn't do per-path AA, relies on the target having MSAA
           target->getDrawState().getStencil().isDisabled();
}

bool GrStencilAndCoverPathRenderer::requiresStencilPass(const SkPath& path,
                                                        GrPathFill fill,
                                                        const GrDrawTarget* target) const {
    return true;
}

void GrStencilAndCoverPathRenderer::drawPathToStencil(const SkPath& path,
                                                      GrPathFill fill,
                                                      GrDrawTarget* target) {
    GrAssert(kEvenOdd_GrPathFill == fill || kWinding_GrPathFill == fill);
    SkAutoTUnref<GrPath> p(fGpu->createPath(path));
    target->stencilPath(p, fill);
}

bool GrStencilAndCoverPathRenderer::onDrawPath(const SkPath& path,
                                               GrPathFill fill,
                                               GrDrawTarget* target,
                                               bool antiAlias) {
    GrAssert(!antiAlias);
    GrAssert(kHairLine_GrPathFill != fill);

    GrDrawState* drawState = target->drawState();
    GrAssert(drawState->getStencil().isDisabled());

    SkAutoTUnref<GrPath> p(fGpu->createPath(path));

    GrPathFill nonInvertedFill = GrNonInvertedFill(fill);
    target->stencilPath(p, nonInvertedFill);

    // TODO: Use built in cover operation rather than a rect draw. This will require making our
    // fragment shaders be able to eat varyings generated by a matrix.

    // fill the path, zero out the stencil
    GrRect bounds = p->getBounds();
    SkScalar bloat = drawState->getViewMatrix().getMaxStretch() * SK_ScalarHalf;
    GrDrawState::AutoDeviceCoordDraw adcd;

    if (nonInvertedFill == fill) {
        GR_STATIC_CONST_SAME_STENCIL(kStencilPass,
            kZero_StencilOp,
            kZero_StencilOp,
            kNotEqual_StencilFunc,
            0xffff,
            0x0000,
            0xffff);
        *drawState->stencil() = kStencilPass;
    } else {
        GR_STATIC_CONST_SAME_STENCIL(kInvertedStencilPass,
            kZero_StencilOp,
            kZero_StencilOp,
            // We know our rect will hit pixels outside the clip and the user bits will be 0
            // outside the clip. So we can't just fill where the user bits are 0. We also need to
            // check that the clip bit is set.
            kEqualIfInClip_StencilFunc,
            0xffff,
            0x0000,
            0xffff);
        SkMatrix vmi;
        bounds.setLTRB(0, 0,
                       SkIntToScalar(drawState->getRenderTarget()->width()),
                       SkIntToScalar(drawState->getRenderTarget()->height()));
        // mapRect through persp matrix may not be correct
        if (!drawState->getViewMatrix().hasPerspective() && drawState->getViewInverse(&vmi)) {
            vmi.mapRect(&bounds);
            // theoretically could set bloat = 0, instead leave it because of matrix inversion
            // precision.
        } else {
            adcd.set(drawState);
            bloat = 0;
        }
        *drawState->stencil() = kInvertedStencilPass;
    }
    bounds.outset(bloat, bloat);
    target->drawSimpleRect(bounds, NULL);
    target->drawState()->stencil()->setDisabled();
    return true;
}
