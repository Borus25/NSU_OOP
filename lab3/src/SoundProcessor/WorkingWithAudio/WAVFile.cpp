#include "WAVFile.h"
#include "../utils/Exceptions.h"
#include <memory>
#include <iostream>

WAVFile::WAVFile()
    : isInput_(false), isOutput_(false), dataStartPosition_(0)
{
}

WAVFile::~WAVFile() {
    close();
}

void WAVFile::open(const std::string& filename) {
    filename_ = filename;

    inputFile_ = std::make_shared<std::ifstream>(filename, std::ios::binary);

    if (!inputFile_ || !inputFile_->is_open()) {
        throw FileException("Cannot open file for reading: " + filename);
    }

    try {
        header_.readHeader(*inputFile_);
    } catch (const SPException& e) {
        inputFile_->close();
        throw;
    }

    dataStartPosition_ = inputFile_->tellg();
    stream_ = std::make_shared<AudioStream>(inputFile_, header_.getDataSize(), dataStartPosition_);

    isInput_ = true;
    isOutput_ = false;
}

void WAVFile::createOutput(const std::string& filename) {
    filename_ = filename;

    outputFile_ = std::make_shared<std::ofstream>(
        filename,
        std::ios::binary | std::ios::out | std::ios::trunc
    );

    if (!outputFile_ || !outputFile_->is_open()) {
        throw FileException("Cannot open file for writing: " + filename);
    }

    if (!outputFile_->good()) {
        throw FileException("Output file is in bad state: " + filename);
    }

    header_.createDefaultHeader(0);
    header_.writeHeader(*outputFile_, 0);

    dataStartPosition_ = outputFile_->tellp();

    std::shared_ptr<std::ostream> outputStream =
        std::static_pointer_cast<std::ostream>(outputFile_);

    stream_ = std::make_shared<AudioStream>(outputStream);

    isInput_ = false;
    isOutput_ = true;
}


void WAVFile::close() {
    if (isOutput_ && outputFile_ && outputFile_->is_open()) {
        finalizeOutput();
        outputFile_->close();
    }

    if (isInput_ && inputFile_ && inputFile_->is_open()) {
        inputFile_->close();
    }

    isInput_ = false;
    isOutput_ = false;
}

const WAVHeader& WAVFile::getHeader() const {
    return header_;
}

std::shared_ptr<AudioStream> WAVFile::getStream() {
    return stream_;
}

std::shared_ptr<AudioStream> WAVFile::createNewStream() {
    if (!isInput_) {
        throw FileException("Cannot create new stream from non-input file");
    }

    if (!inputFile_ || !inputFile_->is_open()) {
        throw FileException("Input file is not open");
    }

    return std::make_shared<AudioStream>(inputFile_, header_.getDataSize(), dataStartPosition_);
}

bool WAVFile::isOpen() const {
    return isInput_ || isOutput_;
}

bool WAVFile::isInputFile() const {
    return isInput_;
}

bool WAVFile::isOutputFile() const {
    return isOutput_;
}

std::string WAVFile::getFilename() const {
    return filename_;
}

void WAVFile::finalizeOutput() {
    if (!isOutput_ || !outputFile_ || !outputFile_->is_open()) {
        return;
    }

    uint32_t writtenSamples = stream_->getWrittenSamples();
    uint32_t dataSize = writtenSamples * 2;

    outputFile_->seekp(0);
    header_.createDefaultHeader(dataSize);
    header_.writeHeader(*outputFile_, dataSize);
    outputFile_->flush();
}

