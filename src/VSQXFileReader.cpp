#include <libvsq/VSQXFileReader.hpp>
#include <libvsq/InputStream.hpp>
#include <libvsq/StringUtil.hpp>
#include <sstream>
#include <cmath>
#include <iterator>
#include <tinyxml2.h>

#define forEachXmlChildElementWithTagName(_parent, _var, _tag) \
    for (auto _var = (_parent)->FirstChildElement(_tag); \
        _var != nullptr; \
        _var = _var->NextSiblingElement(_tag))

LIBVSQ_BEGIN_NAMESPACE

class VSQXFileReader::Impl
{
public:
	void read(Sequence& sequence, InputStream& stream) const
	{
		using namespace tinyxml2;

		std::vector<char> all;
		std::vector<char> buffer(512);
		while (true) {
			size_t const read = stream.read(buffer.data(), 0, buffer.size());
			std::copy(buffer.begin(), buffer.begin() + read,
					  std::back_inserter(all));
			if (read < buffer.size()) {
				break;
			}
		}
		
		XMLDocument doc;
		XMLError err = doc.Parse(all.data(), all.size());
		std::vector<char>().swap(all);
		std::vector<char>().swap(buffer);

		if (err != XML_SUCCESS) {
			return;
		}
		
		XMLElement const* root = doc.RootElement();
		std::string const rootName(root->Name());
		if (rootName.size() < 4) {
			return;
		}
		int const version = std::stoi(std::string(rootName.begin() + 3, rootName.end()));
		if (version != 3 && version != 4) {
			return;
		}
        bool const isv3 = version == 3;

		char const* const tag_masterTrack = "masterTrack";
		char const* const tag_preMeasure  = "preMeasure";
		char const* const tag_timeSig     = "timeSig";
		char const* const tag_posMes      =               isv3 ? "posMes" :      "m";
		char const* const tag_nume        =               isv3 ? "nume" :        "nu";
		char const* const tag_denomi      =               isv3 ? "denomi" :      "de";
		char const* const tag_tempo       = "tempo";
		char const* const tag_posTick     =               isv3 ? "posTick" :     "t";
		char const* const tag_bpm         =               isv3 ? "bpm" :         "v";
		char const* const tag_vsTrack     = "vsTrack";
		char const* const tag_musicalPart =               isv3 ? "musicalPart" : "vsPart";
		char const* const tag_note        = "note";
		char const* const tag_durTick     =               isv3 ? "durTick" :     "dur";
		char const* const tag_noteNum     =               isv3 ? "noteNum" :     "n";
		char const* const tag_lyric       =               isv3 ? "lyric" :       "y";
		char const* const tag_phnms       =               isv3 ? "phnms" :       "p";
		char const* const tag_velocity    =               isv3 ? "velocity" :    "v";
		char const* const tag_noteStyle   =               isv3 ? "noteStyle" :   "nStyle";
		char const* const tag_attr        =               isv3 ? "attr" :        "v";
		char const* const tag_seqAttr     =               isv3 ? "seqAttr" :     "seq";
		char const* const tag_elem        =               isv3 ? "elem" :        "cc";
		char const* const tag_posNrm      =               isv3 ? "posNrm" :      "p";
		char const* const tag_elv         =               isv3 ? "elv" :         "v";
		char const* const tag_mCtrl       =               isv3 ? "mCtrl" :       "cc";
		char const* const tag_mixer       = "mixer";
		char const* const tag_masterUnit  = "masterUnit";
		char const* const tag_vol         = "vol";
		char const* const tag_vsUnit      = "vsUnit";
		char const* const tag_vsTrackNo   =               isv3 ? "vsTrackNo" :   "tNo";
		char const* const tag_pan         = "pan";
		char const* const tag_solo        =               isv3 ? "solo" :        "s";
		char const* const tag_mute        =               isv3 ? "mute" :        "m";
		char const* const tag_singer      = "singer";
		char const* const tag_vBS         =               isv3 ? "vBS" :         "bs";
		char const* const tag_vPC         =               isv3 ? "vPC" :         "pc";
		
		char const* const attr_id = "id";
		
		char const* const cc_DYN = isv3 ? "DYN" : "D";
		char const* const cc_BRE = isv3 ? "BRE" : "B";
		char const* const cc_BRI = isv3 ? "BRI" : "R";
		char const* const cc_CLE = isv3 ? "CLE" : "C";
		char const* const cc_GEN = isv3 ? "GEN" : "G";
		char const* const cc_PIT = isv3 ? "PIT" : "P";
		char const* const cc_PBS = isv3 ? "PBS" : "S";
		char const* const cc_POR = isv3 ? "POR" : "T";
		
		XMLElement const* master = root->FirstChildElement(tag_masterTrack);
		if (master != nullptr) {
			XMLElement const* preMeasure = master->FirstChildElement(tag_preMeasure);
			if (preMeasure != nullptr) {
				preMeasure->QueryIntText(&sequence.master.preMeasure);
			}
			
			sequence.timesigList.clear();
			forEachXmlChildElementWithTagName(master, timeSig, tag_timeSig) {
				XMLElement const* posMes = timeSig->FirstChildElement(tag_posMes);
				if (posMes == nullptr) {
					continue;
				}
				XMLElement const* nume = timeSig->FirstChildElement(tag_nume);
				if (nume == nullptr) {
					continue;
				}
				XMLElement const* denomi = timeSig->FirstChildElement(tag_denomi);
				if (denomi == nullptr) {
					continue;
				}
				Timesig t;
				if (posMes->QueryIntText(&t.barCount) == XML_SUCCESS &&
					nume->QueryIntText(&t.numerator) == XML_SUCCESS &&
					denomi->QueryIntText(&t.denominator) == XML_SUCCESS) {
					sequence.timesigList.push(t);
				}
			}
			
			sequence.tempoList.clear();
			forEachXmlChildElementWithTagName(master, tempo, tag_tempo) {
				XMLElement const* posTick = tempo->FirstChildElement(tag_posTick);
				if (posTick == nullptr) {
					continue;
				}
				XMLElement const* bpm = tempo->FirstChildElement(tag_bpm);
				if (bpm == nullptr) {
					continue;
				}
				Tempo t;
				int64_t tick;
				int64_t tempo100x;
				if (posTick->QueryInt64Text(&tick) == XML_SUCCESS &&
					bpm->QueryInt64Text(&tempo100x) == XML_SUCCESS) {
					t.tick = tick;
					t.tempo = int64_t(60) * int64_t(1000000) * int64_t(100) / tempo100x;
					sequence.tempoList.push(t);
				}
			}
		}

		sequence.tracks().clear();
		sequence.mixer.slave.clear();
		
		// vsTrack
		forEachXmlChildElementWithTagName(root, vsTrack, tag_vsTrack) {
			Track t;
			t.events().clear();

			forEachXmlChildElementWithTagName(vsTrack, musicalPart, tag_musicalPart) {
				XMLElement const* posTick = musicalPart->FirstChildElement(tag_posTick);
				if (posTick == nullptr) {
					continue;
				}
				int64_t offset;
				if (posTick->QueryInt64Text(&offset) != XML_SUCCESS) {
					continue;
				}
				
				// singer
				forEachXmlChildElementWithTagName(musicalPart, singer, tag_singer) {
					XMLElement const* posTick = singer->FirstChildElement(tag_posTick);
					XMLElement const* vBS = singer->FirstChildElement(tag_vBS);
					XMLElement const* vPC = singer->FirstChildElement(tag_vPC);
					if (posTick == nullptr || vBS == nullptr || vPC == nullptr) {
						continue;
					}
					
					int64_t tmp;
					if (posTick->QueryInt64Text(&tmp) != XML_SUCCESS) {
						continue;
					}
					Event e(tmp + offset, EventType::SINGER);
					if (vBS->QueryIntText(&e.singerHandle.language) != XML_SUCCESS ||
						vPC->QueryIntText(&e.singerHandle.program) != XML_SUCCESS) {
						continue;
					}
					
					t.events().add(e);
				}
				
				forEachXmlChildElementWithTagName(musicalPart, note, tag_note) {
					int64_t tmp;

					// posTick
					XMLElement const* notePosTick = note->FirstChildElement(tag_posTick);
					if (notePosTick == nullptr) {
						continue;
					}
					if (notePosTick->QueryInt64Text(&tmp) != XML_SUCCESS) {
						continue;
					}
					Event e(tmp + offset, EventType::NOTE);
					Handle vib(HandleType::VIBRATO);

					// durTick
					XMLElement const* durTick = note->FirstChildElement(tag_durTick);
					if (durTick == nullptr) {
						continue;
					}
					if (durTick->QueryInt64Text(&tmp) != XML_SUCCESS) {
						continue;
					}
					e.length(tmp);

					// noteNum
					XMLElement const* noteNum = note->FirstChildElement(tag_noteNum);
					if (noteNum == nullptr) {
						continue;
					}
					if (noteNum->QueryInt64Text(&tmp) != XML_SUCCESS) {
						continue;
					}
					e.note = tmp;
					
					// lyric
					XMLElement const* lyric = note->FirstChildElement(tag_lyric);
					if (lyric == nullptr) {
						continue;
					}
					std::string const l = lyric->GetText();
					
					// phnms
					XMLElement const* phnms = note->FirstChildElement(tag_phnms);
					if (phnms == nullptr) {
						continue;
					}
					std::string const p = phnms->GetText();
					
					Lyric y(l, p);
					e.lyricHandle.set(0, y);
					
					// velocity
					XMLElement const* velocity = note->FirstChildElement(tag_velocity);
					if (velocity == nullptr) {
						continue;
					}
					if (velocity->QueryInt64Text(&tmp) != XML_SUCCESS) {
						continue;
					}
					e.dynamics = tmp;
					
					// noteStyle (optional)
					e.pmbPortamentoUse = 0;
					XMLElement const* noteStyle = note->FirstChildElement(tag_noteStyle);
					if (noteStyle != nullptr) {
						// attr
						forEachXmlChildElementWithTagName(noteStyle, attr, tag_attr) {
							char const* n = attr->Attribute(attr_id);
							if (n == nullptr) {
								continue;
							}
							int v;
							if (attr->QueryIntText(&v) != XML_SUCCESS) {
								continue;
							}
							std::string const name = n;
							if (name == "accent") {
								e.demAccent = v;
							} else if (name == "bendDep") {
								e.pmBendDepth = v;
							} else if (name == "bendLen") {
								e.pmBendLength = v;
							} else if (name == "decay") {
								e.demDecGainRate = v;
							} else if (name == "fallPort") {
								if (v > 0) {
									e.pmbPortamentoUse += 2;
								}
							} else if (name == "risePort") {
								if (v > 0) {
									e.pmbPortamentoUse += 1;
								}
							} else if (name == "vibLen") {
								vib.length(std::min((tick_t)std::round(e.length() * v / 100.0), e.length()));
							} else if (name == "vibType") {
								vib.iconId = Handle::getIconIdPrefixVibrato() + StringUtil::toString(v, "%04X");
							}
						}
						
						// seqAttr
						forEachXmlChildElementWithTagName(noteStyle, seqAttr, tag_seqAttr) {
							char const* n = seqAttr->Attribute(attr_id);
							if (n == nullptr) {
								continue;
							}
							std::string const name(n);
							VibratoBPList* destination = nullptr;
							if (name == "vibDep") {
								destination = &vib.depthBP;
							} else if (name == "vibRate") {
								destination = &vib.rateBP;
							}
							if (destination == nullptr) {
								continue;
							}
							std::ostringstream ss;
							int count = 0;
							forEachXmlChildElementWithTagName(seqAttr, elem, tag_elem) {
								XMLElement const* posNrm = elem->FirstChildElement(tag_posNrm);
								XMLElement const* elv = elem->FirstChildElement(tag_elv);
								if (posNrm == nullptr || elv == nullptr) {
									continue;
								}
								int t, v;
								if (posNrm->QueryIntText(&t) != XML_SUCCESS ||
									elv->QueryIntText(&v) != XML_SUCCESS) {
									continue;
								}
								// x := normalized distance from the beginning of the note (max 1.0)
								double const x = t / 65536.0;
								tick_t const vibLength = std::max(tick_t(1), e.vibratoHandle.length());
								tick_t const vibStart = std::max(tick_t(0), e.length() - vibLength);
								
								// time: normalized distance from the beginning of the vibrato (max 1.0)
								float const time = std::max(0.0, (e.length() * x - vibStart) / double(vibLength));
								if (count > 0) {
									ss << ",";
								}
								ss << StringUtil::toString(time, "%.06f") << "=" << v;
								count++;
							}
							destination->data(ss.str());
						}
					}

					if (!vib.iconId.empty()) {
						e.vibratoHandle = vib;
					}
					t.events().add(e);
				}
				
				forEachXmlChildElementWithTagName(musicalPart, mCtrl, tag_mCtrl) {
					XMLElement const* posTick = mCtrl->FirstChildElement(tag_posTick);
					XMLElement const* attr = mCtrl->FirstChildElement(tag_attr);
					if (posTick == nullptr || attr == nullptr) {
						continue;
					}
					char const* n = attr->Attribute(attr_id);
					if (n == nullptr) {
						continue;
					}
					std::string name(n);
					BPList* destination = nullptr;
					if (name == cc_DYN) {
						destination = t.curve("DYN");
					} else if (name == cc_BRE) {
						destination = t.curve("BRE");
					} else if (name == cc_BRI) {
						destination = t.curve("BRI");
					} else if (name == cc_CLE) {
						destination = t.curve("CLE");
					} else if (name == cc_GEN) {
						destination = t.curve("GEN");
					} else if (name == cc_PBS) {
						destination = t.curve("PBS");
					} else if (name == cc_PIT) {
						destination = t.curve("PIT");
					} else if (name == cc_POR) {
						destination = t.curve("POR");
					}
					if (destination == nullptr) {
						continue;
					}
					int v;
					if (attr->QueryIntText(&v) != XML_SUCCESS) {
						continue;
					}
					int t;
					if (posTick->QueryIntText(&t) != XML_SUCCESS) {
						continue;
					}
					destination->add(t + offset, v);
				}
			}
			sequence.tracks().push_back(t);
			sequence.mixer.slave.push_back(MixerItem(0, 0, 0, 0));
		}
		
		// mixer
		XMLElement const* mixer = root->FirstChildElement(tag_mixer);
		if (mixer != nullptr) {
			// masterUnit
			XMLElement const* masterUnit = mixer->FirstChildElement(tag_masterUnit);
			if (masterUnit != nullptr) {
				XMLElement const* vol = masterUnit->FirstChildElement(tag_vol);
				if (vol != nullptr) {
					int v;
					if (vol->QueryIntText(&v) == XML_SUCCESS) {
						sequence.mixer.masterFeder = v;
					}
				}
			}
			
			// vsUnit
			forEachXmlChildElementWithTagName(mixer, vsUnit, tag_vsUnit) {
				XMLElement const* vsTrackNo = vsUnit->FirstChildElement(tag_vsTrackNo);
				if (vsTrackNo == nullptr) {
					continue;
				}
				int trackNo;
				if (vsTrackNo->QueryIntText(&trackNo) != XML_SUCCESS) {
					continue;
				}
				if (trackNo < 0 || sequence.mixer.slave.size() <= trackNo) {
					continue;
				}
				XMLElement const* vol = vsUnit->FirstChildElement(tag_vol);
				if (vol != nullptr) {
					int v;
					if (vol->QueryIntText(&v) == XML_SUCCESS) {
						sequence.mixer.slave[trackNo].feder = v;
					}
				}
				XMLElement const* pan = vsUnit->FirstChildElement(tag_pan);
				if (pan != nullptr) {
					int v;
					if (vol->QueryIntText(&v) == XML_SUCCESS) {
						sequence.mixer.slave[trackNo].panpot = v;
					}
				}
				XMLElement const* mute = vsUnit->FirstChildElement(tag_mute);
				if (mute != nullptr) {
					int v;
					if (vol->QueryIntText(&v) == XML_SUCCESS) {
						sequence.mixer.slave[trackNo].mute = v;
					}
				}
				XMLElement const* solo = vsUnit->FirstChildElement(tag_solo);
				if (mute != nullptr) {
					int v;
					if (solo->QueryIntText(&v) == XML_SUCCESS) {
						sequence.mixer.slave[trackNo].solo = v;
					}
				}
			}
		}
	}
};


VSQXFileReader::VSQXFileReader()
	: _impl(std::make_unique<Impl>())
{}



VSQXFileReader::~VSQXFileReader()
{}


void VSQXFileReader::read(Sequence& sequence, InputStream& stream) const
{
	_impl->read(sequence, stream);
}

LIBVSQ_END_NAMESPACE
