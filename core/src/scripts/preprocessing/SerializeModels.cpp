#include "Model.h"
#include "env.h"

#include <filesystem>
#include <vector>

struct ModelImport {
    std::string srcFile;
    std::string outputName;
};
int main() {
    // assimpPath, model Name

    std::vector<ModelImport> MODELS = {{"backpack/backpack.obj", "backpack"}, {"cube/cube.obj", "cube"}};
    return serializeModels(MODELS);
}

int serializeModels(const std::vector<ModelImport>& models) { // models = (name, path/to/assimp)

    for(ModelImport model: models) {
        std::string srcPath = env::RAW_MODEL_DIR + model.srcFile;
        std::string outputPath = env::PREPROCCESSED_MODEL_DIR + model.outputName + ".model";
        if(std::filesystem::exists(outputPath)) {
            continue;
        }
        Model m = Model.importAssimp(srcPath);
        m.serialize(outputPath);
    }
    return 0;
}
