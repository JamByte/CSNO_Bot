#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "discord.h"
#include "cee-utils.h" /* cee_timestamp_ms() */

void on_ready(struct discord *client, const struct discord_user *bot) {
  log_info("Embed-Bot succesfully connected to Discord as %s#%s!",
      bot->username, bot->discriminator);
}



void on_builder_init(
  struct discord *client, 
  const struct discord_user *bot, 
  const struct discord_message *msg)
{
  if (msg->author->bot) return;

  struct discord_embed embed={0};
  discord_embed_set_title(&embed, "%s", "Orca");
  snprintf(embed.description, sizeof(embed.description), "Multi-REST Api library");
  embed.url = strdup("https://github.com/cee-studio/orca");
  embed.color = 3447003;
  discord_embed_set_footer(
    &embed, 
    "github.com/cee-studio/orca",
    "https://raw.githubusercontent.com/cee-studio/orca-docs/master/docs/source/images/icon.svg",
    NULL);
  discord_embed_set_image(
    &embed, 
    "https://github.com/cee-studio/orca-docs/blob/master/docs/source/images/social-preview.png?raw=true",
    NULL, 
    0, 0);
  discord_embed_set_author(
    &embed, 
    "cee-studio",
    "https://github.com/cee-studio",
    "https://cee.dev/static/images/cee.png",
    NULL);
  discord_embed_add_field(
    &embed,
    "Want to learn more?",
    "Read our [documentation](https://cee-studio.github.io/orca/discord_api.html#c.discord_embed)!",
    false);
  discord_embed_add_field(
    &embed,
    "Need help troubleshooting?",
    "Debug with [Saiph-C](https://www.cee.studio/)",
    false);
  discord_embed_add_field(
    &embed,
    "Looking for support?",
    "Join our server [here](https://discord.gg/x4hhGQYu)!",
    false);

  struct discord_create_message_params params = { .embed = &embed };
  discord_create_message(client, msg->channel_id, &params, NULL);

  discord_embed_cleanup(&embed);
}

int main(int argc, char *argv[])
{
  const char *config_file;
  if (argc > 1)
    config_file = argv[1];
  else
    config_file = "bot.config";

  discord_global_init();
  struct discord *client = discord_config_init(config_file);
  assert(NULL != client && "Couldn't initialize client");

  discord_set_on_ready(client, &on_ready);

  discord_set_prefix(client, "!");
  discord_set_on_command(client, "embedtest", &on_builder_init);

  printf("\n\nThis bot demonstrates how to embeds"
         " with three different methods.\n"
         "1 - From JSON init (type !from_json_init): This is the easiest method by far, you can use it"
         " with a JSON library of your preference.\n"
         "2 - Designated init (type !designated_init): This is a 'clean' initialization approach"
         " but is not very flexible.\n"
         "3 - Builder init (type !builder_init): This is a very flexible approach, it relies on utility functions from discord-misc.c.\n"
         "\nTYPE ANY KEY TO START BOT\n");
  fgetc(stdin); // wait for input

  discord_run(client);

  discord_cleanup(client);
  discord_global_cleanup();
}

