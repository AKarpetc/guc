#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <guc.h>

void print_usage()
{
  fprintf(stderr, "Usage: guc <gltf_path> <usd_path> [params]\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Available params:\n");
  fprintf(stderr, "--emit-mtlx                       Emit MaterialX materials in addition to UsdPreviewSurfaces\n");
  fprintf(stderr, "--mtlx-as-usdshade                Convert and inline MaterialX materials with UsdMtlx\n");
  fprintf(stderr, "--flatten-nodes                   Flatten MaterialX glTF PBR nodes to stdlib and pbrlib nodes\n");
  fprintf(stderr, "--explicit-colorspace-transforms  Explicitly transform colorspaces using MaterialX nodes\n");
  fprintf(stderr, "--hdstorm-compat                  Apply compatibility tweaks for the USD hdStorm renderer\n");
}

int main(int argc, const char* argv[])
{
  if (argc < 3)
  {
    print_usage();
    return EXIT_FAILURE;
  }

  const char* gltf_path = argv[1];
  const char* usd_path = argv[2];

  struct guc_params params;
  params.emit_mtlx = false;
  params.mtlx_as_usdshade = false;
  params.flatten_nodes = false;
  params.explicit_colorspace_transforms = false;
  params.hdstorm_compat = false;

  for (int i = 3; i < argc; i++)
  {
    const char* arg = argv[i];

    if (strlen(arg) > 2)
    {
      arg += 2;

      if (!strcmp(arg, "emit-mtlx"))
      {
        params.emit_mtlx = true;
        continue;
      }
      else if (!strcmp(arg, "mtlx-as-usdshade"))
      {
        params.mtlx_as_usdshade = true;
        continue;
      }
      else if (!strcmp(arg, "flatten-nodes"))
      {
        params.flatten_nodes = true;
        continue;
      }
      else if (!strcmp(arg, "explicit-colorspace-transforms"))
      {
        params.explicit_colorspace_transforms = true;
        continue;
      }
      else if (!strcmp(arg, "hdstorm-compat"))
      {
        params.hdstorm_compat = true;
        continue;
      }
    }

    print_usage();
    return EXIT_FAILURE;
  }

  bool result = guc_convert(gltf_path, usd_path, &params);

  return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
