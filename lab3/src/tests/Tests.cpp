#include "test_runner.h"
#include "Tests.h"
#include "../SoundProcessor/SoundProcessor.h"
#include "../SoundProcessor/utils/ConfigParser.h"
#include "../SoundProcessor/utils/ConverterCommand.h"
#include "../SoundProcessor/Convertion/VolumeConverter/VolumeConverter.h"
#include "../SoundProcessor/Convertion/MuteConverter/MuteConverter.h"
#include "../SoundProcessor/Convertion/MixConverter/MixConverter.h"
#include "../SoundProcessor/WorkingWithAudio/WAVHeader.h"
#include "../SoundProcessor/WorkingWithAudio/AudioStream.h"
#include "../SoundProcessor/utils/Exceptions.h"

#include <sstream>
#include <vector>
#include <memory>
#include <fstream>
#include <cstring>


void TestConfigParserSimple() {
    std::string filename = "test_config_simple.txt";
    std::ofstream out(filename);
    out << "# Comment\n";
    out << "mute 10 20\n";
    out << "volume 1.5\n";
    out.close();

    ConfigParser parser;
    parser.parse(filename);
    const auto& commands = parser.getCommands();

    ASSERT_EQUAL(commands.size(), 2u);

    ASSERT_EQUAL(commands[0].getName(), "mute");
    ASSERT_EQUAL(commands[0].getParameters().size(), 2u);
    ASSERT_EQUAL(commands[0].getParameters()[0], "10");

    ASSERT_EQUAL(commands[1].getName(), "volume");
    ASSERT_EQUAL(commands[1].getParameters()[0], "1.5");

    std::remove(filename.c_str());
}

void TestConfigParserErrors() {
    ConfigParser parser;
    ASSERT_THROWS(parser.parse("non_existent.txt"), SPException);
}


void TestConverterCommandUtils() {
    ASSERT(ConverterCommand::isStreamReference("$1"));
    ASSERT(ConverterCommand::isStreamReference("$10"));
    ASSERT(!ConverterCommand::isStreamReference("$"));
    ASSERT(!ConverterCommand::isStreamReference("1"));

    ASSERT_EQUAL(ConverterCommand::getStreamNumber("$5"), 5);
    ASSERT_THROWS(ConverterCommand::getStreamNumber("5"), std::invalid_argument);

    ASSERT(ConverterCommand::isInteger("123"));
    ASSERT(ConverterCommand::isInteger("-10"));
    ASSERT(!ConverterCommand::isInteger("12.5"));
    ASSERT(!ConverterCommand::isInteger("abc"));

    ASSERT(ConverterCommand::isFloat("12.5"));
    ASSERT(ConverterCommand::isFloat("-0.5"));
}


void TestVolumeConverterParameters() {
    VolumeConverter vc;

    vc.setParameters({"0.5", "10", "20"});

    ASSERT_THROWS(vc.setParameters({"-1.0"}), ConverterException);

    ASSERT_THROWS(vc.setParameters({"1.0", "20", "10"}), ConverterException);

    ASSERT_THROWS(vc.setParameters({}), ConverterException);
}

void TestMuteConverterParameters() {
    MuteConverter mc;

    mc.setParameters({"0", "10"});

    ASSERT_THROWS(mc.setParameters({"20", "10"}), ConverterException);

    ASSERT_THROWS(mc.setParameters({"-5", "10"}), ConverterException);
}

void TestWAVHeaderValidation() {
    WAVHeader header;
    header.createDefaultHeader(1000);

    try {
        header.validate();
    } catch (...) {
        ASSERT(false);
    }

    ASSERT_EQUAL(header.getDuration(), 1000.0 / (44100 * 2));
}

void CreateEmptyWAV(const std::string& filename) {
    WAVHeader header;
    header.createDefaultHeader(0);
    std::ofstream file(filename, std::ios::binary);
    header.writeHeader(file, 0);
    file.close();
}

void TestCommandLineArgs() {
    SoundProcessor sp;

    std::ofstream("conf.txt").close();
    CreateEmptyWAV("in.wav");

    char* argv3[] = {(char*)"prog", (char*)"-c", (char*)"conf.txt", (char*)"out.wav", (char*)"in.wav"};
    sp.parseCommandLine(5, argv3);

    ASSERT(sp.isConfigured());

    std::remove("conf.txt");
    std::remove("in.wav");
    std::remove("out.wav");
}

void TestAudioStreamLogic() {
    std::string data = "testdata";
    auto input = std::make_shared<std::stringstream>(data);

    AudioStream stream(input, data.length(), 0);

    ASSERT_EQUAL(stream.getTotalSamples(), 4u);
    ASSERT_EQUAL(stream.getCurrentSampleIndex(), 0u);
    ASSERT(stream.hasMoreSamples());

    int16_t sample = stream.readSample();
    ASSERT_EQUAL(stream.getCurrentSampleIndex(), 1u);

    stream.seekToSecond(0.0);
    ASSERT_EQUAL(stream.getCurrentSampleIndex(), 0u);

    stream.skipSamples(2);
    ASSERT_EQUAL(stream.getCurrentSampleIndex(), 2u);

    stream.reset();
    ASSERT_EQUAL(stream.getCurrentSampleIndex(), 0u);
}

void TestWAVHeaderReadWrite() {
    WAVHeader originalHeader;
    originalHeader.createDefaultHeader(1024);

    std::stringstream buffer;
    originalHeader.writeHeader(buffer, 1024);

    buffer.seekp(0, std::ios::end);
    ASSERT_EQUAL((size_t)buffer.tellp(), 44u);

    buffer.seekg(0);
    WAVHeader readHeader;
    readHeader.readHeader(buffer);

    ASSERT_EQUAL(readHeader.getDataSize(), 1024u);
    ASSERT_EQUAL(readHeader.getSampleRate(), 44100u);

    readHeader.validate();
}

void TestWAVHeaderInvalidFormat() {
    WAVHeader header;
    header.createDefaultHeader(100);


    std::stringstream ss;
    header.writeHeader(ss, 100);

    ss.seekp(20);
    uint16_t badFormat = 6;
    ss.write(reinterpret_cast<char*>(&badFormat), 2);

    ss.seekg(0);
    WAVHeader badHeader;
    badHeader.readHeader(ss);

    try {
        badHeader.validate();
        {
            std::ostringstream os;
            os << "validate() should have thrown InvalidFormatException";
            throw std::runtime_error(os.str());
        }
    } catch (const InvalidFormatException&) {
    } catch (const SPException&) {
    } catch (...) {
        throw std::runtime_error("validate() threw wrong exception type");
    }
}


void TestMixConverterParameters() {
    MixConverter mc;
    ASSERT_THROWS(mc.setParameters({"10"}), ConverterException);
}


void TestConfigParserComplex() {
    std::string filename = "test_config_complex.txt";
    std::ofstream out(filename);
    out << "   # Indented comment\n";
    out << "mix $2 5   \n";
    out << "\n";
    out << "volume   2.0   10   20\n";
    out.close();

    ConfigParser parser;
    parser.parse(filename);
    const auto& commands = parser.getCommands();

    ASSERT_EQUAL(commands.size(), 2u);

    ASSERT_EQUAL(commands[0].getName(), "mix");
    ASSERT_EQUAL(commands[0].getParameters()[0], "$2");

    ASSERT_EQUAL(commands[1].getName(), "volume");
    ASSERT_EQUAL(commands[1].getParameters().size(), 3u);
    ASSERT_EQUAL(commands[1].getParameters()[1], "10");

    std::remove(filename.c_str());
}
void TestAudioStreamErrors() {
    std::shared_ptr<std::istream> nullInput;
    ASSERT_THROWS(AudioStream(nullInput, 100, 0), AudioStreamException);

    auto outSS = std::make_shared<std::stringstream>();
    AudioStream outStream(outSS);
    ASSERT_THROWS(outStream.readSample(), AudioStreamException);
    ASSERT_THROWS(outStream.skipSamples(1), AudioStreamException);
    ASSERT_THROWS(outStream.seekToSecond(1.0), AudioStreamException);

    auto inSS = std::make_shared<std::stringstream>("data");
    AudioStream inStream(inSS, 4, 0);
    ASSERT_THROWS(inStream.writeSample(123), AudioStreamException);
}


void TestMixConverterLogic() {
    MixConverter mc;
    mc.setInsertPosition(0);

    int16_t sampleVal = 1000;
    char rawData[4];
    rawData[0] = static_cast<char>(sampleVal & 0xFF);
    rawData[1] = static_cast<char>((sampleVal >> 8) & 0xFF);
    rawData[2] = rawData[0];
    rawData[3] = rawData[1];

    auto mainSS = std::make_shared<std::stringstream>(std::string(rawData, 4));
    AudioStream mainStream(mainSS, 4, 0);

    int16_t mixVal = 2000;
    char mixData[4];
    mixData[0] = static_cast<char>(mixVal & 0xFF);
    mixData[1] = static_cast<char>((mixVal >> 8) & 0xFF);
    mixData[2] = mixData[0];
    mixData[3] = mixData[1];

    auto mixSS = std::make_shared<std::stringstream>(std::string(mixData, 4));
    auto additionalStream = std::make_shared<AudioStream>(mixSS, 4, 0);

    mc.setAdditionalStream(additionalStream);

    auto outSS = std::make_shared<std::stringstream>();
    AudioStream outStream(outSS);

    mc.process(mainStream, outStream);

    ASSERT_EQUAL(outStream.getWrittenSamples(), 2u);

    std::string result = outSS->str();
    ASSERT_EQUAL(result.size(), 4u);

    int16_t resSample = static_cast<uint8_t>(result[0]) | (static_cast<uint8_t>(result[1]) << 8);
    ASSERT_EQUAL(resSample, 1500);
}

void TestMixConverterStreamLengthMismatch() {
    MixConverter mc;
    mc.setInsertPosition(0);


    auto mainSS1 = std::make_shared<std::stringstream>(std::string(8, 0));


    auto mainInput = std::make_shared<std::stringstream>("1234");
    AudioStream mainStream(mainInput, 4, 0);

    auto addInput = std::make_shared<std::stringstream>("12");
    auto addStream = std::make_shared<AudioStream>(addInput, 2, 0);

    mc.setAdditionalStream(addStream);

    std::stringstream outSS;
    auto outPtr = std::make_shared<std::stringstream>();
    AudioStream outStream(outPtr);

    try {
        mc.process(mainStream, outStream);
    } catch (...) {
        ASSERT(false);
    }

    ASSERT_EQUAL(outStream.getWrittenSamples(), 2u);
}

void TestMixConverterInsertPosition() {
    MixConverter mc;
    mc.setInsertPosition(1);

    auto mainInput = std::make_shared<std::stringstream>("12");
    AudioStream mainStream(mainInput, 2, 0);

    auto addInput = std::make_shared<std::stringstream>("34");
    mc.setAdditionalStream(std::make_shared<AudioStream>(addInput, 2, 0));

    auto outPtr = std::make_shared<std::stringstream>();
    AudioStream outStream(outPtr);

    mc.process(mainStream, outStream);

    ASSERT_EQUAL(outStream.getWrittenSamples(), 1u);
}


void TestSoundProcessorArgsFull() {
    SoundProcessor sp;
    std::ofstream("conf.txt").close();
    CreateEmptyWAV("in1.wav");

    char* argv4[] = {(char*)"prog", (char*)"-c", (char*)"conf.txt", (char*)"out.wav", (char*)"in1.wav"};
    sp.parseCommandLine(5, argv4);

    ASSERT(sp.isConfigured());

    std::remove("conf.txt");
    std::remove("in1.wav");
}

void TestAudioStreamReadFail() {
    auto ss = std::make_shared<std::stringstream>();
    ss->write("1234", 4);
    ss->seekg(0);

    AudioStream stream(ss, 100, 0);

    stream.readSample();
    stream.readSample();

    ASSERT_THROWS(stream.readSample(), AudioStreamException);
}

void TestAudioStreamWriteFail() {

    auto ss = std::make_shared<std::stringstream>();
    AudioStream stream(ss);

    stream.writeSample(123);
    ASSERT_EQUAL(stream.getWrittenSamples(), 1u);

    ss->setstate(std::ios::badbit);
    ASSERT_THROWS(stream.writeSample(123), AudioStreamException);
}


void TestConfigParserMissingArgs() {
    std::string filename = "test_config_bad.txt";
    std::ofstream out(filename);
    out << "mute\n";
    out.close();

    ConfigParser parser;
    parser.parse(filename);

    const auto& commands = parser.getCommands();
    ASSERT_EQUAL(commands.size(), 1u);
    ASSERT_EQUAL(commands[0].getName(), "mute");
    ASSERT(commands[0].getParameters().empty());

    std::remove(filename.c_str());
}

void TestWAVHeaderBadValues() {
    WAVHeader header;
    header.createDefaultHeader(100);

    std::stringstream ss;
    header.writeHeader(ss, 100);

    ss.seekp(24);
    uint32_t badRate = 48000;
    ss.write(reinterpret_cast<char*>(&badRate), 4);

    ss.seekg(0);
    WAVHeader badHeader;
    badHeader.readHeader(ss);

    try {
        badHeader.validate();
        throw std::runtime_error("validate() (BadSampleRate) did not throw");
    } catch (const InvalidFormatException&) {
    } catch (const SPException&) {
    }
}


void TestSoundProcessorRunErrors() {
    SoundProcessor sp;

    char* argv1[] = {(char*)"prog", (char*)"-c", (char*)"missing.txt", (char*)"out.wav", (char*)"in.wav"};


    int result = sp.run(5, argv1);
    ASSERT_EQUAL(result, 5);
}

void TestWAVFileLifecycle() {
    std::string filename = "test_lifecycle.wav";

    {
        WAVFile file;
        file.createOutput(filename);
        ASSERT(file.isOutputFile());
        ASSERT(!file.isInputFile());
        ASSERT(file.isOpen());

        auto stream = file.getStream();
        stream->writeSample(100);
    }

    {
        WAVFile file;
        file.open(filename);
        ASSERT(file.isInputFile());
        ASSERT(file.getHeader().getDataSize() == 2);

        ASSERT_THROWS(file.createOutput("other.wav"), FileException);
    }

    std::remove(filename.c_str());
}

void TestWAVFileErrors() {
    WAVFile file;
    ASSERT(!file.getStream());

    ASSERT_THROWS(file.createNewStream(), FileException);
}



void TestWAVHeaderChunkSkipping() {
    std::stringstream ss;

    ss.write("RIFF", 4);
    uint32_t totalSize = 36 + 8 + 12;
    ss.write(reinterpret_cast<char*>(&totalSize), 4);
    ss.write("WAVE", 4);

    ss.write("JUNK", 4);
    uint32_t junkSize = 4;
    ss.write(reinterpret_cast<char*>(&junkSize), 4);
    ss.write("1234", 4);

    ss.write("fmt ", 4);
    uint32_t fmtSize = 16;
    ss.write(reinterpret_cast<char*>(&fmtSize), 4);
    uint16_t audioFmt = 1;
    ss.write(reinterpret_cast<char*>(&audioFmt), 2);
    uint16_t numChan = 1;
    ss.write(reinterpret_cast<char*>(&numChan), 2);
    uint32_t sampleRate = 44100;
    ss.write(reinterpret_cast<char*>(&sampleRate), 4);
    uint32_t byteRate = 88200;
    ss.write(reinterpret_cast<char*>(&byteRate), 4);
    uint16_t blockAlign = 2;
    ss.write(reinterpret_cast<char*>(&blockAlign), 2);
    uint16_t bitsPerSample = 16;
    ss.write(reinterpret_cast<char*>(&bitsPerSample), 2);

    ss.write("LIST", 4);
    uint32_t listSize = 4;
    ss.write(reinterpret_cast<char*>(&listSize), 4);
    ss.write("INFO", 4);

    ss.write("data", 4);
    uint32_t dataSize = 0;
    ss.write(reinterpret_cast<char*>(&dataSize), 4);

    ss.seekg(0);

    WAVHeader header;
    try {
        header.readHeader(ss);
    } catch (...) {
        ASSERT(false);
    }

    ASSERT_EQUAL(header.getSampleRate(), 44100u);
}


void TestSoundProcessorUnknownConverter() {
    std::string configName = "bad_conv_config.txt";
    std::ofstream out(configName);
    out << "super_echo 10\n";
    out.close();

    SoundProcessor sp;
    char* argv[] = {(char*)"prog", (char*)"-c", (char*)configName.c_str(), (char*)"out.wav", (char*)"in.wav"};


    int res = sp.run(5, argv);
    ASSERT(res != 0);

    std::remove(configName.c_str());
}


void TestConverterCommandEdgeCases() {
    ConverterCommand cmd("cmd", {"p1"});
    ASSERT_THROWS(cmd.getParameter(1), std::out_of_range);

    ASSERT(!ConverterCommand::isInteger(""));
    ASSERT(!ConverterCommand::isInteger(" "));
    ASSERT(!ConverterCommand::isInteger("12a"));
    ASSERT(ConverterCommand::isInteger("0"));

    ASSERT(!ConverterCommand::isFloat(""));
    ASSERT(!ConverterCommand::isFloat("."));
    ASSERT(ConverterCommand::isFloat("0.0"));
    ASSERT(ConverterCommand::isFloat(".5"));

    ASSERT(!ConverterCommand::isStreamReference(""));
    ASSERT(!ConverterCommand::isStreamReference("$"));
    ASSERT(!ConverterCommand::isStreamReference("$a"));
    ASSERT(ConverterCommand::isStreamReference("$0"));
}


void TestVolumeConverterLogic() {
    VolumeConverter vc;
    vc.setVolumeFactor(2.0f);

    std::stringstream inSS;
    int16_t data[] = {1000, 10000, 25000};
    inSS.write(reinterpret_cast<char*>(data), sizeof(data));
    inSS.seekg(0);

    AudioStream inStream(std::make_shared<std::stringstream>(inSS.str()), sizeof(data), 0);

    auto outBuffer = std::make_shared<std::stringstream>();
    AudioStream outStream(outBuffer);

    vc.process(inStream, outStream);

    std::string resStr = outBuffer->str();

    ASSERT_EQUAL(resStr.size(), 6u);

    auto getSample = [&](size_t idx) -> int16_t {
        if (idx * 2 + 1 >= resStr.size()) return 0;
        uint8_t low = static_cast<uint8_t>(resStr[idx * 2]);
        uint8_t high = static_cast<uint8_t>(resStr[idx * 2 + 1]);
        return static_cast<int16_t>(low | (high << 8));
    };

    ASSERT_EQUAL(getSample(0), 2000);
    ASSERT_EQUAL(getSample(1), 20000);
    ASSERT_EQUAL(getSample(2), 32767);
}



void TestConfigParserFormatting() {
    std::string filename = "test_fmt.txt";
    std::ofstream out(filename);
    out << "\n";
    out << "   # Comment with spaces  \n";
    out << "  mute    10    20   \n";
    out << "volume 0.5\n";
    out.close();

    ConfigParser parser;
    parser.parse(filename);

    const auto& cmds = parser.getCommands();
    ASSERT_EQUAL(cmds.size(), 2u);
    ASSERT_EQUAL(cmds[0].getName(), "mute");
    ASSERT_EQUAL(cmds[0].getParameters()[1], "20");

    std::remove(filename.c_str());
}


void TestFullProcessingData() {

    SoundProcessor sp;

    std::ofstream conf("test_mute.txt");
    conf << "mute 0 1\n";
    conf.close();

    WAVHeader header;
    header.createDefaultHeader(88200);
    std::ofstream wav("test_input.wav", std::ios::binary);
    header.writeHeader(wav, 88200);
    int16_t sample = 10000;
    for(int i=0; i<44100; ++i) {
        wav.write(reinterpret_cast<char*>(&sample), 2);
    }
    wav.close();

    char* argv[] = {(char*)"prog", (char*)"-c", (char*)"test_mute.txt", (char*)"test_output.wav", (char*)"test_input.wav"};
    sp.run(5, argv);

    WAVFile outFile;
    outFile.open("test_output.wav");
    auto stream = outFile.getStream();

    for(int i=0; i<10; ++i) {
        if(stream->readSample() != 0) {
            ASSERT(false);
        }
    }

    std::remove("test_mute.txt");
    std::remove("test_input.wav");
    std::remove("test_output.wav");
}


void TestSoundProcessorInputValidation() {
    SoundProcessor sp;

    std::ofstream conf("bad_ref.txt");
    conf << "mix $100 0\n";
    conf.close();

    char* argv[] = {(char*)"prog", (char*)"-c", (char*)"bad_ref.txt", (char*)"out.wav", (char*)"in.wav"};
    int res = sp.run(5, argv);
    ASSERT_EQUAL(res, 2);

    std::remove("bad_ref.txt");
}

void TestSoundProcessorNoInputFiles() {
    SoundProcessor sp;
    std::ofstream conf("simple.txt");
    conf << "mute 0 1\n";
    conf.close();


    char* argv[] = {(char*)"prog", (char*)"-c", (char*)"simple.txt", (char*)"out.wav", (char*)"non_existent.wav"};
    int res = sp.run(5, argv);
    ASSERT_EQUAL(res, 2);

    std::remove("simple.txt");
}


void TestAudioStreamOutputOps() {
    auto ss = std::make_shared<std::stringstream>();
    AudioStream out(ss);

    ASSERT_THROWS(out.reset(), AudioStreamException);
    ASSERT_THROWS(out.seekToSecond(1.0), AudioStreamException);
    ASSERT_THROWS(out.skipSamples(10), AudioStreamException);

    out.writeSample(10);
    ASSERT_EQUAL(out.getCurrentSampleIndex(), 1u);
}


void TestMixConverterNoStream() {
    MixConverter mc;

    auto inSS = std::make_shared<std::stringstream>("12");
    AudioStream in(inSS, 2, 0);

    auto outSS = std::make_shared<std::stringstream>();
    AudioStream out(outSS);

    ASSERT_THROWS(mc.process(in, out), ConverterException);
}


void TestALL() {
    TestRunner tr;
    RUN_TEST(tr, TestConfigParserSimple);
    RUN_TEST(tr, TestConfigParserErrors);
    RUN_TEST(tr, TestConfigParserMissingArgs);
    RUN_TEST(tr, TestConfigParserFormatting);

    RUN_TEST(tr, TestConverterCommandUtils);
    RUN_TEST(tr, TestConverterCommandEdgeCases);

    RUN_TEST(tr, TestVolumeConverterParameters);
    RUN_TEST(tr, TestVolumeConverterLogic);

    RUN_TEST(tr, TestMuteConverterParameters);

    RUN_TEST(tr, TestMixConverterParameters);
    RUN_TEST(tr, TestMixConverterLogic);
    RUN_TEST(tr, TestMixConverterStreamLengthMismatch);
    RUN_TEST(tr, TestMixConverterInsertPosition);
    RUN_TEST(tr, TestMixConverterNoStream);

    RUN_TEST(tr, TestWAVHeaderValidation);
    RUN_TEST(tr, TestWAVHeaderReadWrite);
    RUN_TEST(tr, TestWAVHeaderChunkSkipping);
    RUN_TEST(tr, TestWAVFileLifecycle);
    RUN_TEST(tr, TestWAVFileErrors);

    RUN_TEST(tr, TestAudioStreamLogic);
    RUN_TEST(tr, TestAudioStreamErrors);
    RUN_TEST(tr, TestAudioStreamReadFail);
    RUN_TEST(tr, TestAudioStreamWriteFail);
    RUN_TEST(tr, TestAudioStreamOutputOps);

    RUN_TEST(tr, TestCommandLineArgs);
    RUN_TEST(tr, TestSoundProcessorArgsFull);
    RUN_TEST(tr, TestSoundProcessorRunErrors);
    RUN_TEST(tr, TestSoundProcessorUnknownConverter);
    RUN_TEST(tr, TestSoundProcessorInputValidation);
    RUN_TEST(tr, TestSoundProcessorNoInputFiles);

    RUN_TEST(tr, TestFullProcessingData);
}