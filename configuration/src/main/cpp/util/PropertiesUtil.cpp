/*   Copyright (C) 2013-2015 Computer Sciences Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <ezbake/configuration/util/PropertiesUtil.h>

#include <stdexcept>
#include <utility>
#include <streambuf>

#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

namespace ezbake { namespace configuration { namespace util {

const ::std::string PropertiesUtil::PROPERTY_FILE_EXTENSION = "properties";

/**
 * A properties file parser taken from log4cxx implementation
 */
class PropertyParser {
public:
    void parse(::std::string& in, PropertiesUtil::Properties& properties) {
        ::std::string key, element;
        LexemType lexemType = BEGIN;
        char c;
        bool finished = false;

        if (!get(in, c)) {
            return;
        }

        while (!finished) {
            switch(lexemType) {
            case BEGIN:
                switch(c) {
                    case 0x20: // ' '
                    case 0x08: // '\t'
                    case 0x0A: // '\n'
                    case 0x0D: // '\r'
                        if (!get(in, c))
                            finished = true;
                        break;

                    case 0x23: // '#'
                    case 0x21: // '!'
                        lexemType = COMMENT;
                        if (!get(in, c))
                            finished = true;
                        break;

                    default:
                        lexemType = KEY;
                        break;
                }
                break;

            case KEY:
                switch(c) {
                case 0x5C: // '\\'
                    lexemType = KEY_ESCAPE;
                    if (!get(in, c))
                        finished = true;
                    break;

                case 0x08: // '\t'
                case 0x20: // ' '
                case 0x3A: // ':'
                case 0x3D: // '='
                    lexemType = DELIMITER;
                    if (!get(in, c))
                        finished = true;
                    break;

                case 0x0A:
                case 0x0D:
                    // key associated with an empty string element
                    properties[key] = std::string();
                    key.erase(key.begin(), key.end());
                    lexemType = BEGIN;
                    if (!get(in, c))
                        finished = true;
                    break;

                default:
                    key.append(1, c);
                    if (!get(in, c))
                        finished = true;
                    break;
                }
                break;

            case KEY_ESCAPE:
                switch(c) {
                case 0x08: // '\t'
                case 0x20: // ' '
                case 0x3A: // ':'
                case 0x3D: // '='
                case 0x5C: // '\\'
                    key.append(1, c);
                    lexemType = KEY;
                    if (!get(in, c))
                        finished = true;
                    break;

                case 0x0A: // '\n'
                    lexemType = KEY_CONTINUE;
                    if (!get(in, c))
                        finished = true;
                    break;

                case 0x0D: // '\r'
                    lexemType = KEY_CONTINUE2;
                    if (!get(in, c))
                        finished = true;
                    break;
                }
                break;

            case KEY_CONTINUE:
                switch(c) {
                case 0x20:  // ' '
                case 0x08: //  '\t'
                    if (!get(in, c))
                        finished = true;
                    break;

                default:
                    lexemType = KEY;
                    break;
                }
                break;

            case KEY_CONTINUE2:
                switch(c) {
                case 0x0A: // '\n'
                    if (!get(in, c))
                        finished = true;
                    lexemType = KEY_CONTINUE;
                    break;

                default:
                    lexemType = KEY_CONTINUE;
                    break;
                }
                break;

            case DELIMITER:
                switch(c) {
                case 0x08: // '\t'
                case 0x20: // ' '
                case 0x3A: // ':'
                case 0x3D: // '='
                    if (!get(in, c))
                        finished = true;
                    break;

                default:
                    lexemType = ELEMENT;
                    break;
                }
                break;

            case ELEMENT:
                switch(c) {
                case 0x5C: // '\\'
                    lexemType = ELEMENT_ESCAPE;
                    if (!get(in, c))
                        finished = true;
                    break;

                case 0x0A: // '\n'
                case 0x0D: // '\r'
                    // key associated with an empty string element
                    properties[key] = element;
                    key.erase(key.begin(), key.end());
                    element.erase(element.begin(), element.end());
                    lexemType = BEGIN;
                    if (!get(in, c))
                        finished = true;
                    break;

                default:
                    element.append(1, c);
                    if (!get(in, c))
                        finished = true;
                    break;
                }
                break;

            case ELEMENT_ESCAPE:
                switch(c) {
                case 0x08: // '\t'
                case 0x20: // ' '
                case 0x6E: // 'n'
                case 0x72: // 'r'
                case 0x27: // '\''
                case 0x5C: // '\\'
                case 0x22: // '\"'
                case 0x3A: // ':'
                default:
                    element.append(1, c);
                    lexemType = ELEMENT;
                    if (!get(in, c))
                        finished = true;
                    break;

                case 0x0A: // '\n'
                    lexemType = ELEMENT_CONTINUE;
                    if (!get(in, c))
                        finished = true;
                    break;

                case 0x0D: // '\r'
                    lexemType = ELEMENT_CONTINUE2;
                    if (!get(in, c))
                        finished = true;
                    break;
                }
                break;

            case ELEMENT_CONTINUE:
                switch(c) {
                case 0x20: // ' '
                case 0x08: // '\t'
                    if (!get(in, c))
                        finished = true;
                    break;

                default:
                    lexemType = ELEMENT;
                    break;
                }
                break;

            case ELEMENT_CONTINUE2:
                switch(c) {
                case 0x20: // '\n'
                    if (!get(in, c))
                        finished = true;
                    lexemType = ELEMENT_CONTINUE;
                    break;

                default:
                    lexemType = ELEMENT_CONTINUE;
                    break;
                }
                break;

            case COMMENT:
                if (c == 0x0A || c == 0x0D) {
                    lexemType = BEGIN;
                }
                if (!get(in, c))
                    finished = true;
                break;
            }
        }

        if (!key.empty()) {
            properties[key] = element;
        }
    }

protected:
    bool get(::std::string& in, char& c) {
        if (in.empty()) {
            c = 0;
            return false;
        }

        c = in[0];
        in.erase(in.begin());
        return true;
    }

    enum LexemType {
        BEGIN,
        KEY,
        KEY_ESCAPE,
        KEY_CONTINUE,
        KEY_CONTINUE2,
        DELIMITER,
        ELEMENT,
        ELEMENT_ESCAPE,
        ELEMENT_CONTINUE,
        ELEMENT_CONTINUE2,
        COMMENT
    };
};

bool PropertiesUtil::isEmpty(const Properties& props) {
    return stringPropertyNames(props).empty();
}

void PropertiesUtil::checkForCollisions(const Properties& toCheckAgainst, const Properties& toCheck) {
    for (auto itr = toCheck.begin(); itr != toCheck.end(); itr++) {
        if (toCheckAgainst.find(itr->first) != toCheckAgainst.end()) {
            BOOST_THROW_EXCEPTION(::std::runtime_error(itr->first + " has already been defined"));
        }
    }
}

void PropertiesUtil::mergeProperties(
        Properties& mergeWith, const Properties& toMerge, bool shouldOverride) {
    if (!shouldOverride) {
        checkForCollisions(mergeWith, toMerge);
    }

    mergeWith.insert(toMerge.begin(), toMerge.end());
}

void PropertiesUtil::loadPropertiesFromFile(
        const ::std::string& filename, Properties& loadInto, bool shouldOverride) {
    if (::std::string::npos == filename.find_first_not_of(' ')) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("filename parameter cannot be empty"));
    }

    ::std::ifstream filestream(filename);
    if (!filestream.good()) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("Could not load properties from file: " + filename));
    }

    try {
        loadPropertiesFromFileStream(filestream, loadInto, shouldOverride);
    } catch (...) {
        filestream.close();
        throw;
    }
    filestream.close();
}

void PropertiesUtil::loadPropertiesFromDirectory(
        const ::std::string& directory, Properties& loadInto, bool shouldOverride) {
    if (::std::string::npos == directory.find_first_not_of(' ')) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("directory parameter cannot be empty"));
    }

    ::boost::filesystem::directory_iterator itr(directory);
    for (; itr != ::boost::filesystem::directory_iterator(); itr++) {
        ::std::string filepath = itr->path().string();

        if (!::boost::algorithm::ends_with(filepath, PROPERTY_FILE_EXTENSION)) {
            //ignore files that are not property files
            continue;
        }

        //read properties from file
        loadPropertiesFromFile(filepath, loadInto, shouldOverride);
    }
}

PropertiesUtil::Properties PropertiesUtil::getPropertiesByPrefix(const Properties& from, ::std::string prefix) {
    Properties props;

    ::boost::algorithm::trim(prefix);
    if (prefix.empty()) {
        return props;
    }

    // make sure that we end with a '.'
    //if(prefix.back() != '.') {
    if (prefix.at(prefix.length() - 1) != '.') {
        prefix += '.';
    }

    BOOST_FOREACH(const ::std::string& propertyName, stringPropertyNames(from)) {
        if (::boost::starts_with(propertyName, prefix)) {
            props[propertyName] = from.at(propertyName);
        }
    }

    return props;
}

::std::vector< ::std::string> PropertiesUtil::stringPropertyNames(const Properties& props) {
    ::std::vector< ::std::string> names;

    //reserve enough space to avoid dynamically reallocating
    names.reserve(props.size());

    ::std::pair< ::std::string, ::std::string> entry;
    BOOST_FOREACH(entry, props) {
        names.push_back(entry.first);
    }
    return names;
}

void PropertiesUtil::loadPropertiesFromFileStream(
        ::std::ifstream& stream, Properties& loadInto, bool shouldOverride) {
    mergeProperties(loadInto, readPropertiesFromFileStream(stream), shouldOverride);
}

PropertiesUtil::Properties PropertiesUtil::readPropertiesFromFileStream(::std::ifstream& stream) {
    if (!stream.is_open()) {
        BOOST_THROW_EXCEPTION(::std::runtime_error("file input stream not opened"));
    }

    Properties props;
    ::std::string contents;

    //preallocate enough space for contents
    stream.seekg(0, std::ios::end);
    contents.reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);

    //read contents
    contents.assign((::std::istreambuf_iterator<char>(stream)),
                     ::std::istreambuf_iterator<char>());

    PropertyParser parser;
    parser.parse(contents, props);

    return props;
}

}}} // namespace ::ezbake::configuration::util
