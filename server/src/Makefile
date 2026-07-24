all:
	echo "make <kor | kortest | jpn / jpnep | mal | tld | twn | usa | brz | hbk | ger | eur | rus | mex | espusa | frausa | brzep | mexep | uk>"
	echo "kor     : connector/helper/messenger/loginserver(release) and gameserver(debug)"
	echo "kortest : connector/helper/messenger/loginserver(test) and gameserver(test debug)"
	echo "other   : connector/helper/messenger/loginserver(release) and gameserver(debug)"

kor:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib kor && \
	make -j4 -C ./GameServer kordebug && \
	make -j4 -C ./Connector kor && \
	make -j4 -C ./Helper kor && \
	make -j4 -C ./SubHelper kor && \
	make -j4 -C ./Messenger kor && \
	make -j4 -C ./LoginServer kor
#	version_up sub
	
kortest:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib kor && \
	make -j4 -C ./GameServer kortestdebug && \
	make -j4 -C ./Connector kortestdebug && \
	make -j4 -C ./Helper kortestdebug && \
	make -j4 -C ./SubHelper kortestdebug && \
	make -j4 -C ./Messenger kortestdebug && \
	make -j4 -C ./LoginServer kortestdebug

korqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib kor && \
	make -j4 -C ./GameServer korqadebug && \
	make -j4 -C ./Connector korqa && \
	make -j4 -C ./Helper korqa && \
	make -j4 -C ./SubHelper korqa && \
	make -j4 -C ./Messenger korqa && \
	make -j4 -C ./LoginServer korqa

kordep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer kordep && \
	make -j4 -C ./Connector kordep && \
	make -j4 -C ./Helper kordep && \
	make -j4 -C ./SubHelper kordep && \
	make -j4 -C ./Messenger kordep && \
	make -j4 -C ./LoginServer kordep

tld:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib tld && \
	make -j4 -C ./GameServer tlddebug && \
	make -j4 -C ./Connector tld && \
	make -j4 -C ./Helper tld && \
	make -j4 -C ./SubHelper tld && \
	make -j4 -C ./Messenger tld && \
	make -j4 -C ./LoginServer tld

tldqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib tld && \
	make -j4 -C ./GameServer tldqadebug && \
	make -j4 -C ./Connector tldqa && \
	make -j4 -C ./Helper tldqa && \
	make -j4 -C ./SubHelper tldqa && \
	make -j4 -C ./Messenger tldqa && \
	make -j4 -C ./LoginServer tldqa

tlddep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer tlddep && \
	make -j4 -C ./Connector tlddep && \
	make -j4 -C ./Helper tlddep && \
	make -j4 -C ./SubHelper tlddep && \
	make -j4 -C ./Messenger tlddep && \
	make -j4 -C ./LoginServer tlddep

usa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib usa && \
	make -j4 -C ./GameServer usadebug && \
	make -j4 -C ./Connector usa && \
	make -j4 -C ./Helper usa && \
	make -j4 -C ./SubHelper usa && \
	make -j4 -C ./Messenger usa && \
	make -j4 -C ./LoginServer usa

usaqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib usa && \
	make -j4 -C ./GameServer usaqadebug && \
	make -j4 -C ./Connector usaqa && \
	make -j4 -C ./Helper usaqa && \
	make -j4 -C ./SubHelper usaqa && \
	make -j4 -C ./Messenger usaqa && \
	make -j4 -C ./LoginServer usaqa

usadep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer usadep && \
	make -j4 -C ./Connector usadep && \
	make -j4 -C ./Helper usadep && \
	make -j4 -C ./SubHelper usadep && \
	make -j4 -C ./Messenger usadep && \
	make -j4 -C ./LoginServer usadep

brz:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib brz && \
	make -j4 -C ./GameServer brzdebug && \
	make -j4 -C ./Connector brz && \
	make -j4 -C ./Helper brz && \
	make -j4 -C ./SubHelper brz && \
	make -j4 -C ./Messenger brz && \
	make -j4 -C ./LoginServer brz

brzdep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer brzdep && \
	make -j4 -C ./Connector brzdep && \
	make -j4 -C ./Helper brzdep && \
	make -j4 -C ./SubHelper brzdep && \
	make -j4 -C ./Messenger brzdep && \
	make -j4 -C ./LoginServer brzdep

hbk:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib hbk && \
	make -j4 -C ./GameServer hbkdebug && \
	make -j4 -C ./Connector hbk && \
	make -j4 -C ./Helper hbk && \
	make -j4 -C ./SubHelper hbk && \
	make -j4 -C ./Messenger hbk && \
	make -j4 -C ./LoginServer hbk

hbkqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib hbk && \
	make -j4 -C ./GameServer hbkqadebug && \
	make -j4 -C ./Connector hbkqa && \
	make -j4 -C ./Helper hbkqa && \
	make -j4 -C ./SubHelper hbkqa && \
	make -j4 -C ./Messenger hbkqa && \
	make -j4 -C ./LoginServer hbkqa
	
hbkdep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer hbkdep && \
	make -j4 -C ./Connector hbkdep && \
	make -j4 -C ./Helper hbkdep && \
	make -j4 -C ./SubHelper hbkdep && \
	make -j4 -C ./Messenger hbkdep && \
	make -j4 -C ./LoginServer hbkdep

ger:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib ger && \
	make -j4 -C ./GameServer gerdebug && \
	make -j4 -C ./Connector ger && \
	make -j4 -C ./Helper ger && \
	make -j4 -C ./SubHelper ger && \
	make -j4 -C ./Messenger ger && \
	make -j4 -C ./LoginServer ger

gerdep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer gerdep && \
	make -j4 -C ./Connector gerdep && \
	make -j4 -C ./Helper gerdep && \
	make -j4 -C ./SubHelper gerdep && \
	make -j4 -C ./Messenger gerdep && \
	make -j4 -C ./LoginServer gerdep

gerqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib ger && \
	make -j4 -C ./GameServer gerdebugqa && \
	make -j4 -C ./Connector gerqa && \
	make -j4 -C ./Helper gerqa && \
	make -j4 -C ./SubHelper gerqa && \
	make -j4 -C ./Messenger gerqa && \
	make -j4 -C ./LoginServer gerqa

spn:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib spn && \
	make -j4 -C ./GameServer spndebug && \
	make -j4 -C ./Connector spn && \
	make -j4 -C ./Helper spn && \
	make -j4 -C ./SubHelper spn && \
	make -j4 -C ./Messenger spn && \
	make -j4 -C ./LoginServer spn

spndep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer spndep && \
	make -j4 -C ./Connector spndep && \
	make -j4 -C ./Helper spndep && \
	make -j4 -C ./SubHelper spndep && \
	make -j4 -C ./Messenger spndep && \
	make -j4 -C ./LoginServer spndep

spnqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib spn && \
	make -j4 -C ./GameServer spndebugqa && \
	make -j4 -C ./Connector spnqa && \
	make -j4 -C ./Helper spnqa && \
	make -j4 -C ./SubHelper spnqa && \
	make -j4 -C ./Messenger spnqa && \
	make -j4 -C ./LoginServer spnqa

frc:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib frc && \
	make -j4 -C ./GameServer frcdebug && \
	make -j4 -C ./Connector frc && \
	make -j4 -C ./Helper frc && \
	make -j4 -C ./SubHelper frc && \
	make -j4 -C ./Messenger frc && \
	make -j4 -C ./LoginServer frc

frcdep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer frcdep && \
	make -j4 -C ./Connector frcdep && \
	make -j4 -C ./Helper frcdep && \
	make -j4 -C ./SubHelper frcdep && \
	make -j4 -C ./Messenger frcdep && \
	make -j4 -C ./LoginServer frcdep

frcqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib frc && \
	make -j4 -C ./GameServer frcdebugqa && \
	make -j4 -C ./Connector frcqa && \
	make -j4 -C ./Helper frcqa && \
	make -j4 -C ./SubHelper frcqa && \
	make -j4 -C ./Messenger frcqa && \
	make -j4 -C ./LoginServer frcqa
	
pld:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib pld && \
	make -j4 -C ./GameServer plddebug && \
	make -j4 -C ./Connector pld && \
	make -j4 -C ./Helper pld && \
	make -j4 -C ./SubHelper pld && \
	make -j4 -C ./Messenger pld && \
	make -j4 -C ./LoginServer pld

plddep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer plddep && \
	make -j4 -C ./Connector plddep && \
	make -j4 -C ./Helper plddep && \
	make -j4 -C ./SubHelper plddep && \
	make -j4 -C ./Messenger plddep && \
	make -j4 -C ./LoginServer plddep

pldqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib pld && \
	make -j4 -C ./GameServer plddebugqa && \
	make -j4 -C ./Connector pldqa && \
	make -j4 -C ./Helper pldqa && \
	make -j4 -C ./SubHelper pldqa && \
	make -j4 -C ./Messenger pldqa && \
	make -j4 -C ./LoginServer pldqa

rus:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib rus && \
	make -j4 -C ./GameServer rusdebug && \
	make -j4 -C ./Connector rus && \
	make -j4 -C ./Helper rus && \
	make -j4 -C ./SubHelper rus && \
	make -j4 -C ./Messenger rus && \
	make -j4 -C ./LoginServer rus
	
rusqa:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib rus && \
	make -j4 -C ./GameServer rusqadebug && \
	make -j4 -C ./Connector rusqa && \
	make -j4 -C ./Helper rusqa && \
	make -j4 -C ./SubHelper rusqa && \
	make -j4 -C ./Messenger rusqa && \
	make -j4 -C ./LoginServer rusqa
	
rusdep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer rusdep && \
	make -j4 -C ./Connector rusdep && \
	make -j4 -C ./Helper rusdep && \
	make -j4 -C ./SubHelper rusdep && \
	make -j4 -C ./Messenger rusdep && \
	make -j4 -C ./LoginServer rusdep

tur:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib tur && \
	make -j4 -C ./GameServer turdebug && \
	make -j4 -C ./Connector tur && \
	make -j4 -C ./Helper tur && \
	make -j4 -C ./SubHelper tur && \
	make -j4 -C ./Messenger tur && \
	make -j4 -C ./LoginServer tur
	
turdep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer turdep && \
	make -j4 -C ./Connector turdep && \
	make -j4 -C ./Helper turdep && \
	make -j4 -C ./SubHelper turdep && \
	make -j4 -C ./Messenger turdep && \
	make -j4 -C ./LoginServer turdep

ita:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib ita && \
	make -j4 -C ./GameServer itadebug && \
	make -j4 -C ./Connector ita && \
	make -j4 -C ./Helper ita && \
	make -j4 -C ./SubHelper ita && \
	make -j4 -C ./Messenger ita && \
	make -j4 -C ./LoginServer ita
	
itadep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer itadep && \
	make -j4 -C ./Connector itadep && \
	make -j4 -C ./Helper itadep && \
	make -j4 -C ./SubHelper itadep && \
	make -j4 -C ./Messenger itadep && \
	make -j4 -C ./LoginServer itadep

mex:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib mex && \
	make -j4 -C ./GameServer mexdebug && \
	make -j4 -C ./Connector mex && \
	make -j4 -C ./Helper mex && \
	make -j4 -C ./SubHelper mex && \
	make -j4 -C ./Messenger mex && \
	make -j4 -C ./LoginServer mex
	
mexdep:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib dep && \
	make -j4 -C ./GameServer mexdep && \
	make -j4 -C ./Connector mexdep && \
	make -j4 -C ./Helper mexdep && \
	make -j4 -C ./SubHelper mexdep && \
	make -j4 -C ./Messenger mexdep && \
	make -j4 -C ./LoginServer mexdep

uk:
	make -j4 -C ./CalcHash && \
	make -j4 -C ./ShareLib uk && \
	make -j4 -C ./GameServer ukdebug && \
	make -j4 -C ./Connector uk && \
	make -j4 -C ./Helper uk && \
	make -j4 -C ./SubHelper uk && \
	make -j4 -C ./Messenger uk && \
	make -j4 -C ./LoginServer uk
	
clean:
	make -j4 -C ./CalcHash clean && \
	make -j4 -C ./ShareLib clean && \
	make -j4 -C ./GameServer clean && \
	make -j4 -C ./Connector clean && \
	make -j4 -C ./Helper clean && \
	make -j4 -C ./SubHelper clean && \
	make -j4 -C ./Messenger clean && \
	make -j4 -C ./LoginServer clean
	
cleanxtrap:
	make -j4 -C ./CalcHash clean && \
	make -j4 -C ./ShareLib clean && \
	make -j4 -C ./GameServer clean && \
	make -j4 -C ./Connector clean && \
	make -j4 -C ./Helper clean && \
	make -j4 -C ./SubHelper clean && \
	make -j4 -C ./Messenger clean && \
	make -j4 -C ./LoginServer clean

cleanall:
	make -j4 -C ./CalcHash clean && \
	make -C ./ShareLib cleanall && \
	make -C ./GameServer cleanall && \
	make -C ./Connector cleanall && \
	make -C ./Helper cleanall && \
	make -C ./SubHelper cleanall && \
	make -C ./Messenger cleanall && \
	make -C ./LoginServer cleanall

cleaneur:
	make -j4 -C ./CalcHash clean && \
	make -j4 -C ./ShareLib clean && \
	make -j4 -C ./GameServer clean && \
	make -j4 -C ./Connector clean && \
	make -j4 -C ./Helper clean && \
	make -j4 -C ./SubHelper clean && \
	make -j4 -C ./Messenger clean && \
	make -j4 -C ./LoginServer clean
