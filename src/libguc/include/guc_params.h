//
// Copyright 2022 Pablo Delgado Krämer
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#ifndef __cplusplus
#include <stdbool.h>
#endif

struct guc_params
{
  // Generate and reference a MaterialX document containing an accurate translation
  // of the glTF materials. The document is serialized to a file if UsdShade inlining
  // is not active.
  bool emit_mtlx;

  // Parse the generated MaterialX document with UsdMtlx to a UsdShade representation
  // and inline it into the USD file. Note that information will be discarded as not
  // all MaterialX concepts can be encoded in UsdShade:
  // https://graphics.pixar.com/usd/release/api/usd_mtlx_page_front.html
  // Files generated without this option may be better supported by future USD
  // versions.
  bool mtlx_as_usdshade;

  // Flatten the glTF PBR bxdf nodes for backwards compatibility with older
  // MaterialX versions (and applications shipped without the appropriate nodedef).
  // This may negatively affect material network parsing and compilation times.
  bool flatten_nodes;

  // MaterialX's 'colorspace' functionality may not be fully supported by an
  // application. We work around this by implementing colorspace transformations using
  // native MaterialX math nodes. MaterialX image nodes are assumed to return raw,
  // untransformed values, since the default document colorspace is 'linear'.
  bool explicit_colorspace_transforms;

  // HdMtlx and therefore Storm do not seem to properly support MaterialX colorspaces.
  // https://github.com/PixarAnimationStudios/USD/issues/1523
  // https://github.com/PixarAnimationStudios/USD/issues/1632
  // To work around this issue, we force-enable explicit colorspace transformations and
  // undo colorspace transformations that exist because of USD's sRGB detection logic:
  // https://github.com/PixarAnimationStudios/USD/blob/857ffda41f4f1553fe1019ac7c7b4f08c233a7bb/pxr/imaging/plugin/hioOiio/oiioImage.cpp#L470-L471
  // Additionally, we make hdStorm recognize alpha materials as translucent.
  bool hdstorm_compat;
};
