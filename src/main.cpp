#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxNetwork.h"
#include "ofAppNoWindow.h"

class ofApp : public ofBaseApp
{
	public:
        ofxTCPServer TCP;
        ofxMidiOut midi;
	int channel, port, mport;

	ofApp(vector<string> conf)
	{
		port    = ofToInt(conf[0]);
		mport   = ofToInt(conf[1]);
		channel = ofToInt(conf[2]);
	}

	void setup()
	{
		TCP.setup(port);
        	TCP.setMessageDelimiter("\n");
		midi.listPorts();
	        midi.openPort(mport);
	}

	void update()
	{
		for(unsigned int i = 0; i < (unsigned int)TCP.getLastID(); i++)
		{
	                if( !TCP.isClientConnected(i) )
			{
				continue;
			}
			else 
			{
			        string str = TCP.receive(i);
				if(str!="")
				{
					vector<string> pars = ofSplitString(str,":");
					string ip    = pars[0];
					int note     = ofToInt(pars[1]);
					int velocity = ofToInt(pars[2]);

					if(velocity > 0)
					{
				                midi.sendNoteOn(channel, note, velocity);
					}
					else
					{
				                midi.sendNoteOff(channel, note, velocity);
					}

			                if(TCP.disconnectClient(i))
					{
						ofLog()<<"disconnect: "+ip;
					}
				}
			}
		}
		ofSleepMillis(5);
	}

	void exit()
	{
		TCP.close();
	        midi.closePort();
	}
};

int main(int argc, char *argv[])
{
	if(argc > 3)
	{
		ofAppNoWindow w;
		ofSetupOpenGL(&w, 1024,768, OF_WINDOW);
		vector<string> conf;
		conf.push_back(argv[1]);
		conf.push_back(argv[2]);
		conf.push_back(argv[3]);
		ofRunApp( new ofApp(conf));
	}
	else
	{
		cout << "Error: parameter[ tcp-port, midi-port, midi-channel ]\n\t./bin/tcpmidiserver 9055 0 1\n";
	}
}
