#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "discord.h"
#include "cee-utils.h" /* cee_timestamp_ms() */

void on_ready(struct discord *client, const struct discord_user *bot) {
  log_info("CSNO_Bot succesfully connected to Discord as %s#%s!",
      bot->username, bot->discriminator);
}



void sendembed(struct discord *client, const struct discord_user *bot, const struct discord_message *msg, char* content)
{

  struct discord_embed embed={0};
  //discord_embed_set_title(&embed, "%s", "CSNO");
  snprintf(embed.description, sizeof(embed.description), content);
  embed.color = (0<<24)|(((rand()&0xff))<<16)|(((rand()&0xff))<<8)|((rand()&0xff));
  

  struct discord_create_message_params params = { .embed = &embed };
  discord_create_message(client, msg->channel_id, &params, NULL);

  discord_embed_cleanup(&embed);
}
void on_message(struct discord *client, const struct discord_user *bot, const struct discord_message *msg) {
  if (0 == strcmp(msg->content, "|ping")) {
    char str[512];
    snprintf(str, sizeof str,"Pong <@%lld>", msg->author->id);
    sendembed(client,bot,msg, str);
  }
}
int main(int argc, char *argv[])
{
  time_t t;
  srand((unsigned) time(&t));
  const char *config_file;
  if (argc > 1)
    config_file = argv[1];
  else
    config_file = "bot.config";

  discord_global_init();
  struct discord *client = discord_config_init(config_file);
  assert(NULL != client && "Couldn't initialize client");

  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_create(client, &on_message);
  discord_run(client);

  discord_cleanup(client);
  discord_global_cleanup();
}

