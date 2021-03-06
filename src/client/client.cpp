#include "client.h"
#include "hooks.h"

#include "string_helpers.h"

namespace shadey {

  void ShadeyClient::onReady(SleepyDiscord::Ready ready) {
    m_self = ready.user;
    updateStatus("Vulkan 1.1");
  }

  void ShadeyClient::onMessage(SleepyDiscord::Message message) {
    try {
      if (message.author.ID == m_self.ID)
        return;

      auto& hooks = ShadeyGlobalHookList::instance()->hooks();
      for (auto& hook : hooks)
        hook->onMessage(*this, message);
    }
    catch (const std::exception& e) {
      std::string exception = e.what();

      if (exception.length() > 1500)
        exception = exception.substr(0, 1500);

      std::string error = "An exception occured: ```" + exception + "```";

      std::cout << error << std::endl;

      try {
        sendMessage(message.channelID, error);
      }
      catch (const std::exception& e) {
        // Do nothing.
      }
    }
  }

}