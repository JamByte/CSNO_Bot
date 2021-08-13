#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "discord.h"
#include <ctype.h>
#include "cee-utils.h" /* cee_timestamp_ms() */
#include <unistd.h>
unsigned long long** authors;
unsigned long long** channels;
unsigned long long* servers;
int serverslength;

int* index;
#define CHARSTOCOPY 50;


void on_ready(struct discord *client, const struct discord_user *bot) {
  log_info("CSNO_Bot succesfully connected to Discord as %s#%s!",
      bot->username, bot->discriminator);
	
	NTL_T(struct discord_guild) guilds = NULL;
	discord_get_current_user_guilds(client, &guilds);
	int i=0;
	  while (guilds[i]) {
		printf("\n%d. %s", i+1, guilds[i]->name);
		++i;
	  }
	  fflush( stdout );
	authors = malloc(i*sizeof(unsigned long long*));
	channels = malloc(i*sizeof(unsigned long long*));
	servers = malloc(i*sizeof(unsigned long long));
	index =malloc(i*sizeof(int));
	serverslength=i;
	int j =0;
	for(j=0;j<i;j++){
		servers[j] = guilds[j]->id;
		authors[j] = malloc(25*sizeof(unsigned long long));
		
		channels[j] = malloc(25*sizeof(unsigned long long));
	}
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
   
    if(msg->content == 0) {return;}
    if(msg->content[0] == '\0') {return;}
	
	int guildindex =0;
	
	for(guildindex=0; guildindex<serverslength; guildindex++){
		if(servers[guildindex] == msg->guild_id){
		break;}
	}
	
	for(int i = 0; msg->content[i]; i++){
	  msg->content[i] = tolower(msg->content[i]);
	}
    if(strstr(msg->content,"https://") == 0 && strstr(msg->content,"http://") == 0 && strstr(msg->content,"nitro") == 0){return;}
    sendembed(client, bot,msg,"I see you sending a link");
    authors[guildindex][index[guildindex]] = msg->author->id;
    channels[guildindex][index[guildindex]] = msg->channel_id;
    index[guildindex]++;
    if(index[guildindex] >24){
        index[guildindex] =0;
    }
    int i=0;
    unsigned long long channel1=0;
    unsigned long long channel2=0;
    for(i=0; i<25; i++){
        if(authors[guildindex][i] == msg->author->id){
            if(channel1 == 0 || channel1 == channels[guildindex][i] ){
                channel1 = channels[guildindex][i];
                continue;
            }
            else if(channel2 == 0|| channel2== channels[guildindex][i]){
                channel2 = channels[guildindex][i];
                continue;
            }
            else
            {
                //ban them !!!
                discord_create_guild_ban(client, msg->guild_id, msg->author->id, 1, "Rule 9: sent too many links");
                sleep(1);
                discord_remove_guild_ban(client, msg->guild_id, msg->author->id, "Rule 9: sent too many links");
            } 
        }
    }

}
int main(int argc, char *argv[])
{


    index=0;
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

