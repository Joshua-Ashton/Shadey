#include "hooks.h"
#include "command_helpers.h"

#include "renderer.h"
#include "string_helpers.h"

namespace shadey {

  class ShaderCodeHook : public ShadeyHook {
  public:
    using ShadeyHook::ShadeyHook;

    void onMessage(ShadeyClient& client, SleepyDiscord::Message message) final {
      // Extract the code from the message.
      std::string code;
      bool hlsl = false;
      {
        std::string content = message.content;

        size_t codeStart = content.find("```glsl");
        if (codeStart == std::string::npos) {
          hlsl = true;
          codeStart = content.find("```hlsl");

          if (codeStart == std::string::npos)
            return;
        }

        content = content.substr(codeStart + strlen("```glsl"));

        size_t codeEnd = content.find("```", codeStart);

        if (codeEnd == std::string::npos)
          return;

        code = content.substr(0, codeEnd);

        if (code.empty())
          return;
      }

      client.sendTyping(message.channelID);

      Renderer renderer;
      std::string filename = renderer.init(hlsl, code);

      try {
        client.uploadFile(message.channelID, filename, "");
      }
      catch (const std::exception& e) {
        throw std::runtime_error("File was too big to upload!");
      }
    }
  };

  SHADEY_REGISTER_HOOK(ShaderCodeHook);

}