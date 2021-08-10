cd ..
cd ..
cc -std=c11 -O0 -g -Wall -Wno-unused-function -I. -I./cee-utils -I./common -I./common/third-party -DLOG_USE_COLOR -Wno-unused-but-set-variable -fPIC -D_XOPEN_SOURCE=700 -I./mujs -o bots/CSNO_Bot/CSNO_Bot.exe bots/CSNO_Bot/CSNO_Bot.c -ldiscord -lgithub -lreddit -lslack -L./lib -lpthread -lm  -lcurl -lcrypto
cd bots 
cp bot.config CSNO_Bot/bot.config
cd CSNO_Bot