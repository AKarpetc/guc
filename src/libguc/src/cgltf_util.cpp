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

#include "cgltf_util.h"

#include <assert.h>
#include <string.h>

namespace guc
{
  const char* cgltf_error_string(cgltf_result result)
  {
    assert(result != cgltf_result_success);
    assert(result != cgltf_result_invalid_options);
    switch (result)
    {
    case cgltf_result_legacy_gltf:
      return "legacy glTF not supported";
    case cgltf_result_data_too_short:
    case cgltf_result_invalid_json:
    case cgltf_result_invalid_gltf:
      return "malformed glTF";
    case cgltf_result_unknown_format:
      return "unknown format";
    case cgltf_result_file_not_found:
      return "file not found";
    case cgltf_result_io_error:
      return "io error";
    case cgltf_result_out_of_memory:
      return "out of memory";
    default:
      return "unknown";
    }
  }

  const cgltf_accessor* cgltf_find_accessor(const cgltf_primitive* primitive,
                                            const char* name)
  {
    for (int j = 0; j < primitive->attributes_count; j++)
    {
      const cgltf_attribute* attribute = &primitive->attributes[j];

      if (strcmp(attribute->name, name) == 0)
      {
        return attribute->data;
      }
    }

    return nullptr;
  }

  // NOTE: the following four functions are copied from the cgltf library and
  // are therefore licensed under its accompanying MIT license.
  cgltf_size cgltf_component_size(cgltf_component_type component_type)
  {
    switch (component_type)
    {
    case cgltf_component_type_r_8:
    case cgltf_component_type_r_8u:
      return 1;
    case cgltf_component_type_r_16:
    case cgltf_component_type_r_16u:
      return 2;
    case cgltf_component_type_r_32u:
    case cgltf_component_type_r_32f:
      return 4;
    case cgltf_component_type_invalid:
    default:
      return 0;
    }
  }

  cgltf_size cgltf_calc_size(cgltf_type type, cgltf_component_type component_type)
  {
    cgltf_size component_size = cgltf_component_size(component_type);
    if (type == cgltf_type_mat2 && component_size == 1)
    {
      return 8 * component_size;
    }
    else if (type == cgltf_type_mat3 && (component_size == 1 || component_size == 2))
    {
      return 12 * component_size;
    }
    return component_size * cgltf_num_components(type);
  }

  int cgltf_unhex(char ch)
  {
    return
      (unsigned)(ch - '0') < 10 ? (ch - '0') :
      (unsigned)(ch - 'A') < 6 ? (ch - 'A') + 10 :
      (unsigned)(ch - 'a') < 6 ? (ch - 'a') + 10 :
      -1;
  }

  cgltf_size cgltf_decode_uri(char* uri)
  {
    char* write = uri;
    char* i = uri;

    while (*i)
    {
      if (*i == '%')
      {
        int ch1 = cgltf_unhex(i[1]);

        if (ch1 >= 0)
        {
          int ch2 = cgltf_unhex(i[2]);

          if (ch2 >= 0)
          {
            *write++ = (char)(ch1 * 16 + ch2);
            i += 3;
            continue;
          }
        }
      }

      *write++ = *i++;
    }

    *write = 0;
    return write - uri;
  }
}
