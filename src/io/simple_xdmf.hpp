#ifndef SIMPLE_XDMF_HPP_INCLUDED
#define SIMPLE_XDMF_HPP_INCLUDED

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#ifdef USE_BOOST
#include <functional>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#endif

class SimpleXdmf {
    private:
        const std::string header = R"(<?xml version="1.0" ?>
<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" []>
)";
        std::string m_content;
        std::string m_buffer;
        std::string m_newLine;
        std::string m_currentXpath = "";
        bool endEdit = false;
        unsigned int innerElementPerLine = 10;


        // indent management
        std::string m_indent;
        unsigned int currentIndentation = -1;
        void addIndent() {
            ++currentIndentation;
        }

        void backIndent() {
            if (currentIndentation > 0) --currentIndentation;
        }

        void insertIndent() {
            for(unsigned int i = 0; i < currentIndentation; ++i) {
                m_buffer += m_indent;
            }
        }


        // store current processing tag information for type validation
        enum class TAG {DataItem, Grid, StructuredTopology, UnstructuredTopology, Geometry, Attribute, Set, Time, Information, Domain, Xdmf, Inner};
        TAG current_tag;

        void setCurrentTag(const std::string& tag) {
            if (tag == "Grid") {
                current_tag = TAG::Grid;
            } else if (tag == "DataItem") {
                current_tag = TAG::DataItem;
            } else if (tag == "StructuredTopology") {
                current_tag = TAG::StructuredTopology;
            } else if (tag == "UnstructuredTopology") {
                current_tag = TAG::UnstructuredTopology;
            } else if (tag == "Geometry") {
                current_tag = TAG::Geometry;
            } else if (tag == "Attribute") {
                current_tag = TAG::Attribute;
            } else if (tag == "Set") {
                current_tag = TAG::Set;
            } else if (tag == "Time") {
                current_tag = TAG::Time;
            } else if (tag == "Domain") {
                current_tag = TAG::Domain;
            } else if (tag == "Information") {
                current_tag = TAG::Information;
            } else if (tag == "Inner") {
                current_tag = TAG::Inner;
            } else if (tag == "Xdmf") {
                current_tag = TAG::Xdmf;
            }
        }


        // Reference Management
        // xpathMap[Name] => Xpath
        std::map<std::string, std::string> xpathMap;

        void addNewXpath(const std::string& name, const std::string& xpath) {
            if (xpathMap.count(name) == 0) {
                xpathMap[name] = xpath;
            }
        }

        std::string getXpath(const std::string& name) {
            if (xpathMap.count(name) > 0) {
                return xpathMap[name] + "[@Name='" + name + "']";
            } else {
                std::cerr << "[SIMPLE XDMF ERROR] Non-existente Name \"" << name << "\" passed to getXpath(). " << std::endl;
                return "";
            }
        }

        void proceedCurrentXpath() {
            m_currentXpath += "/" + getCurrentTagString();
        }

        void regressCurrentXpath() {
            auto last_split_index = m_currentXpath.rfind("/");
            m_currentXpath.erase(m_currentXpath.begin() + last_split_index, m_currentXpath.end());
        }


        // Structure Management
        void beginElement(const std::string& tag) {
            if (m_buffer != "") {
                commitBuffer();
            }

            addIndent();
            insertIndent();
            setCurrentTag(tag);

            m_buffer += "<" + getCurrentTagString();
            proceedCurrentXpath();
        }

        void endElement(const std::string& tag) {
            if (m_buffer != "") {
                commitBuffer();
            }

            insertIndent();
            m_buffer += "</" + convertTagString(tag);
            commitBuffer();

            regressCurrentXpath();
            backIndent();
        }

        void commitBuffer() {
            m_content += m_buffer + ">" + m_newLine;
            m_buffer.clear();
        }

        void endOneLineElement(const std::string& tag) {
            (void) tag;
            if (m_buffer != "") {
                commitOneLineBuffer();
            }
            backIndent();
        }

        void commitOneLineBuffer() {
            m_content += m_buffer + "/>" + m_newLine;
            m_buffer.clear();
        }

        void beginInnerElement() {
            if (m_buffer != "") {
                commitBuffer();
            }
            addIndent();
            insertIndent();

            setCurrentTag("Inner");
        }

        void endInnerElement() {
            if (m_buffer != "") {
                commitInnerBuffer();
            }
            backIndent();
        }

        void commitInnerBuffer() {
            m_content += m_buffer;
            m_buffer.clear();
        }


        // Type checking
        static constexpr int dataItemTypeLength = 6;
        static constexpr int gridTypeLength = 4;
        static constexpr int structuredTopologyTypeLength = 6;
        static constexpr int unstructuredTopologyTypeLength = 17;
        static constexpr int geometryTypeLength = 6;
        static constexpr int attributeTypeLength = 5;
        static constexpr int attributeCenterLength = 5;
        static constexpr int setTypeLength = 4;
        static constexpr int timeTypeLength = 4;
        static constexpr int formatTypeLength = 3;
        static constexpr int numberTypeLength = 5;
        static constexpr int precisionTypeLength = 4;

        std::array<std::string, dataItemTypeLength> DataItemType {{"Uniform", "Collection", "Tree", "HyperSlab", "Coordinates", "Function"}};
        std::array<std::string, gridTypeLength> GridType {{"Uniform", "Collection", "Tree", "Subset"}};
        std::array<std::string, structuredTopologyTypeLength> StructuredTopologyType {{"2DSMesh", "2DRectMesh", "2DCoRectMesh", "3DSMesh", "3DRectMesh", "3DCoRectMesh"}};
        std::array<std::string, unstructuredTopologyTypeLength> UnstructuredTopologyType {{"Polyvertex", "Polyline", "Polygon", "Triangle", "Quadrilateral", "Tetrahedron", "Pyramid", "Wedge", "Hexahedron", "Edge_3", "Tri_6", "Quad_8", "Tet_10", "Pyramid_13", "Wedge_15", "Hex_20", "Mixed"}};
        std::array<std::string, geometryTypeLength> GeometryType {{"XYZ", "VXVY", "X_Y_Z", "VXVYVZ", "ORIGIN_DXDYDZ", "ORIGIN_DXDY"}};
        std::array<std::string, attributeTypeLength> AttributeType {{"Scalar", "Vector", "Tensor", "Tensor6", "Matrix"}};
        std::array<std::string, attributeCenterLength> AttributeCenter {{"Node", "Edge", "Face", "Cell", "Grid"}};
        std::array<std::string, setTypeLength> SetType {{"Node", "Edge", "Face", "Cell"}};
        std::array<std::string, timeTypeLength> TimeType {{"Single", "HyperSlab", "List", "Range"}};
        std::array<std::string, formatTypeLength> FormatType {{"XML", "HDF", "Binary"}};
        std::array<std::string, numberTypeLength> NumberType {{"Float", "Int", "UInt", "Char", "UChar"}};
        std::array<std::string, precisionTypeLength> PrecisionType {{"1", "2", "4", "8"}};

        template<int N>
        bool checkIsValidType(const std::array<std::string, N>& valid_types, const std::string& specified_type) {
            bool is_valid = false;

            for(const auto& t : valid_types) {
                if (t == specified_type) {
                    is_valid = true;
                }
            }

            return is_valid;
        }

        bool checkType(const std::string& type) {
            bool isValid = false;
            switch (current_tag) {
                case TAG::Grid:
                    isValid = checkIsValidType<gridTypeLength>(GridType, type);
                    break;
                case TAG::DataItem:
                    isValid = checkIsValidType<dataItemTypeLength>(DataItemType, type);
                    break;
                case TAG::StructuredTopology:
                    isValid = checkIsValidType<structuredTopologyTypeLength>(StructuredTopologyType, type);
                    break;
                case TAG::UnstructuredTopology:
                    isValid = checkIsValidType<unstructuredTopologyTypeLength>(UnstructuredTopologyType, type);
                    break;
                case TAG::Geometry:
                    isValid = checkIsValidType<geometryTypeLength>(GeometryType, type);
                    break;
                case TAG::Attribute:
                    isValid = checkIsValidType<attributeTypeLength>(AttributeType, type);
                    break;
                case TAG::Set:
                    isValid = checkIsValidType<setTypeLength>(SetType, type);
                    break;
                case TAG::Time:
                    isValid = checkIsValidType<timeTypeLength>(TimeType, type);
                    break;
                case TAG::Information:
                    isValid = true;
                    break;
                case TAG::Domain:
                    isValid = true;
                    break;
                case TAG::Xdmf:
                    isValid = true;
                    break;
                default:
                    break;
            }

            if (!isValid) {
                std::string tagString = getCurrentTagString();

                std::string error_message = "[SIMPLE XDMF ERROR] Invalid " + tagString + " type = " + type + " is passed to.";
                throw std::invalid_argument(error_message);
            }

            return isValid;
        }

        std::string getCurrentTagString() {
            switch (current_tag) {
                case TAG::Grid:
                    return "Grid";
                case TAG::DataItem:
                    return "DataItem";
                case TAG::StructuredTopology:
                    return "Topology";
                case TAG::UnstructuredTopology:
                    return "Topology";
                case TAG::Geometry:
                    return "Geometry";
                case TAG::Attribute:
                    return "Attribute";
                case TAG::Set:
                    return "Set";
                case TAG::Time:
                    return "Time";
                case TAG::Information:
                    return "Information";
                case TAG::Domain:
                    return "Domain";
                case TAG::Inner:
                    return "Inner";
                case TAG::Xdmf:
                    return "Xdmf";
                default:
                    return "";
            }
        }

        std::string convertTagString(const std::string& tag) {
            if (tag == "StructuredTopology" || tag == "UnstructuredTopology") {
                return "Topology";
            } else {
                return tag;
            }
        }

        // Adding Valid Attributes
        void addType(const std::string& type) {
            if (type == "") return;

            if (checkType(type)) {
                auto typeString = getCurrentTagString();
                // irregular naming of data type attribute ....
                if (typeString == "DataItem") typeString = "Item";

                m_buffer += " " + typeString + "Type=\"" + type + "\"";
            }
        }


        // for convinience
        void convertFromVariadicArgsToStringInternal(const std::string& buffer) {
            (void) buffer;
        }

        template<typename First, typename... Rests>
        void convertFromVariadicArgsToStringInternal(std::string& buffer, First&& first, Rests&&... rests) {
            std::stringstream ss;
            ss << first;
            buffer = ss.str() + buffer;

            constexpr std::size_t parameter_pack_size = sizeof...(Rests);
            if (parameter_pack_size > 0) {
                buffer = " " + buffer;
                convertFromVariadicArgsToStringInternal(buffer, std::forward<Rests>(rests)...);
            }
        }

        template<typename... Args>
        std::string convertFromVariadicArgsToString(Args&&... args) {
            std::string buffer = "";
            convertFromVariadicArgsToStringInternal(buffer, std::forward<Args>(args)...);
            return buffer;
        }

        void addItemInternal(std::stringstream& ss) {
            (void) ss;
        }

        template <typename First, typename... Rests>
        void addItemInternal(std::stringstream &ss, First &&first, Rests &&... rests) {
            ss << first;

            constexpr std::size_t parameter_pack_size = sizeof...(Rests);
            if (parameter_pack_size > 0) {
                ss << " ";
                addItemInternal(ss, std::forward<Rests>(rests)...);
            }
        }

    public:
        SimpleXdmf() {
            setNewLineCodeLF();
            setIndentSpaceSize();
            beginXdmf();
        }

        void setIndentSpaceSize(const int size = 4) {
            if (size == 0) {
                m_indent = '\t';
                return;
            }

            m_indent.clear();
            for (int i = 0; i < size; ++i) {
                m_indent += ' ';
            }
        }

        void setNewLineCodeLF() {
            constexpr char LF = '\n';
            m_newLine = LF;
        }

        void setNewLineCodeCR() {
            constexpr char CR = '\r';
            m_newLine = CR;
        }

        void setNewLineCodeCRLF() {
            constexpr const char* CRLF = "\r\n";
            m_newLine = CRLF;
        }

        // IO functions
        void generate(const std::string file_name) {
            if(!endEdit) endXdmf();

            std::ofstream ofs(file_name, std::ios::out);
            ofs << m_content;
        }

        std::string getRawString() const {
            return m_content;
        }

        void beginXdmf() {
            endEdit = false;
            m_content = header;
            beginElement("Xdmf");
        }

        void endXdmf() {
            endElement("Xdmf");
            endEdit = true;
        }

        void beginDomain(const std::string& name = "") {
            beginElement("Domain");
            setName(name);
        };

        void endDomain() {
            endElement("Domain");
        };

        void beginGrid(const std::string& name = "", const std::string& type = "Uniform") {
            beginElement("Grid");
            addType(type);
            setName(name);
        }
        
        void beginGridCollection(const std::string& name = "", const std::string& type = "Uniform", const std::string& collection_type="Spatial") {
            beginElement("Grid");
            addType(type);
            setCollectionType(collection_type);
            setName(name);
        }

        void endGrid() {
            endElement("Grid");
        }

        void beginUnstructuredTopology(const std::string& name = "", const std::string& type = "Polyvertex") {
            beginElement("UnstructuredTopology");
            addType(type);
            setName(name);
        }

        void endUnstructuredTopology() {
            endElement("UnstructuredTopology");
        }

        void beginStructuredTopology(const std::string& name = "", const std::string& type = "2DCoRectMesh") {
            beginElement("StructuredTopology");
            addType(type);
            setName(name);
        }

        void endStructuredTopology() {
            endOneLineElement("StructuredTopology");
        }

        void beginGeometry(const std::string& name = "", const std::string& type = "XYZ"){
            beginElement("Geometry");
            addType(type);
            setName(name);
        }

        void endGeometry(){
            endElement("Geometry");
        }

        void beginAttribute(const std::string& name = "", const std::string& type = "Scalar"){
            beginElement("Attribute");
            addType(type);
            setName(name);
        }

        void endAttribute(){
            endElement("Attribute");
        }

        void beginDataItem(const std::string& name = "", const std::string& type = "Uniform") {
            beginElement("DataItem");
            addType(type);
            setName(name);
        }

        void endDataItem() {
            endElement("DataItem");
        }

        void beginSet(const std::string& name = "", const std::string& type = "Node") {
            beginElement("Set");
            addType(type);
            setName(name);
        }

        void endSet() {
            endElement("Set");
        }

        void beginTime(const std::string& name = "", const std::string& type = "Single") {
            beginElement("Time");
            addType(type);
            setName(name);
        }

        void endTime() {
            endElement("Time");
        }

        void beginInformation(const std::string& name = "") {
            beginElement("Information");
            setName(name);
        }

        void endInformation() {
            endElement("Information");
        }

        template<typename T>
        void addArray(const T* values_ptr, const int N) {
            beginInnerElement();

            std::stringstream ss;
            for(size_t i = 1; i <= N; ++i) {
                ss << values_ptr[i - 1];

                if (i < N) {
                    ss << " ";

                    if (i % innerElementPerLine == 0) {
                        m_buffer += ss.str() + m_newLine;
                        insertIndent();
                        ss.str("");
                        ss.clear(std::stringstream::goodbit);
                    }
                }
            }

            if (ss.str() != "") {
                m_buffer += ss.str() + m_newLine;
            }

            endInnerElement();
        }

        template<typename T>
        void add2DArray(T** values_ptr, const int nx, const int ny) {
            beginInnerElement();

            std::stringstream ss;

            size_t itr = 1;
            size_t size = nx * ny;

            for(size_t j = 1; j <= ny; ++j) {
                for(size_t i = 1; i <= nx; ++i) {
                    ss << values_ptr[i - 1][j - 1];

                    if (itr < size) {
                        ss << " ";

                        if (itr % innerElementPerLine == 0) {
                            m_buffer += ss.str() + m_newLine;
                            insertIndent();
                            ss.str("");
                            ss.clear(std::stringstream::goodbit);
                        }
                    }

                    ++itr;
                }
            }

            if (ss.str() != "") {
                m_buffer += ss.str() + m_newLine;
            }

            endInnerElement();
        }

        template<typename T, size_t N>
        void addArray(const std::array<T, N>& values) {
            beginInnerElement();

            std::stringstream ss;
            for(size_t i = 1; i <= N; ++i) {
                ss << values[i - 1];

                if (i < N) {
                    ss << " ";

                    if (i % innerElementPerLine == 0) {
                        m_buffer += ss.str() + m_newLine;
                        insertIndent();
                        ss.str("");
                        ss.clear(std::stringstream::goodbit);
                    }
                }
            }

            if (ss.str() != "") {
                m_buffer += ss.str() + m_newLine;
            }

            endInnerElement();
        }

        template<typename T>
        void addVector(const std::vector<T>& values) {
            beginInnerElement();

            std::stringstream ss;
            auto size = values.size();
            for(size_t i = 1; i <= size; ++i) {
                ss << values[i - 1];

                if (i < size) {
                    ss << " ";

                    if (i % innerElementPerLine == 0) {
                        m_buffer += ss.str() + m_newLine;
                        insertIndent();
                        ss.str("");
                        ss.clear(std::stringstream::goodbit);
                    }
                }
            }

            if (ss.str() != "") {
                m_buffer += ss.str() + m_newLine;
            }

            endInnerElement();
        }

#ifdef USE_BOOST
        // based on c_index_order (row-major)
        template<typename T, size_t N>
        void addMultiArray(const boost::multi_array<T, N>& values, const bool isFortranStorageOrder = false) {
            const int size = values.num_elements();

            if (isFortranStorageOrder) {
                addArray(values.data(), size);
            } else {
                using array_type = boost::multi_array<T, N>;
                boost::array<typename array_type::index, N> index;

                // initialize index
                for(int i = 0; i < N; ++i) {
                    index[i] = 0;
                }
                auto shape = values.shape();

                // capture index and shape
                std::function<void(int)> proceedIndex = [&index, &shape, &proceedIndex](const int axis){
                    if (axis < N) {
                        // increments from the most left index
                        index[axis] += 1;
                        if (index[axis] == shape[axis]) {
                            index[axis] = 0;
                            proceedIndex(axis + 1);
                        }
                    }
                };

                beginInnerElement();
                std::stringstream ss;
                for (size_t i = 1; i <= size; ++i) {
                    ss << values(index);
                    proceedIndex(0);

                    if (i < size) {
                        ss << " ";

                        if (i % innerElementPerLine == 0) {
                            buffer += ss.str() + newLine;
                            insertIndent();
                            ss.str("");
                            ss.clear(std::stringstream::goodbit);
                        }
                    }
                }

                if (ss.str() != "") {
                    buffer += ss.str() + newLine;
                }

                endInnerElement();
            }
        }
#endif

        template<typename... Args>
        void addItem(Args&&... args) {
            beginInnerElement();

            std::stringstream ss;
            addItemInternal(ss, std::forward<Args>(args)...);
            m_buffer += ss.str() + m_newLine;

            endInnerElement();
        }

        // --- Attirbute Setting Functions ---
        void setName(const std::string& name) {
            if (name != "") {
                addNewXpath(name, m_currentXpath);
                m_buffer += " Name=\"" + name + "\"";
            }
        }

        void setVersion(const std::string& _version) {
            if (current_tag != TAG::Xdmf) {
                std::cerr << "[SIMPLE XDMF ERROR] setVersion() cannot be called when current Tag is not Xdmf." << std::endl;
                return;
            }
            m_buffer += " Version=\"" + _version + "\"";
        }

        void setFormat(const std::string& type = "XML") {
            if (checkIsValidType<formatTypeLength>(FormatType, type)) {
                m_buffer += " Format=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Format type = " + type + " is passed to setFormat().";
                throw std::invalid_argument(error_message);
            }
        }

        void setPrecision(const std::string& type = "4") {
            if (checkIsValidType<precisionTypeLength>(PrecisionType, type)) {
                m_buffer += " Precision=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Precision type = " + type + " is passed to setPrecision().";
                throw std::invalid_argument(error_message);
            }
        }

        void setNumberType(const std::string& type = "Float") {
            if (checkIsValidType<numberTypeLength>(NumberType, type)) {
                m_buffer += " NumberType=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Number type = " + type + " is passed to setNumberType().";
                throw std::invalid_argument(error_message);
            }
        }

        void setCenter(const std::string& type = "Node") {
            if (current_tag != TAG::Attribute) {
                std::cerr << "[SIMPLE XDMF ERROR] setCenter() cannot be called when current Tag is not Attribute." << std::endl;
                return;
            }

            if (checkIsValidType<attributeCenterLength>(AttributeCenter, type)) {
                m_buffer += " Center=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Center type = " + type + " is passed to setCenter().";
                throw std::invalid_argument(error_message);
            }
        }

        void setFunction(const std::string& func) {
            if (current_tag != TAG::DataItem) {
                std::cerr << "[SIMPLE XDMF ERROR] setFunction() cannot be called when current Tag is not DataItem." << std::endl;
                return;
            }
            m_buffer += " Function=\"" + func + "\"";
        }

        void setSection(const std::string& sect) {
            if (current_tag != TAG::Grid) {
                std::cerr << "[SIMPLE XDMF ERROR] setSection() cannot be called when current Tag is not Grid." << std::endl;
                return;
            }

            if (sect == "DataItem" || sect == "All") {
                m_buffer += " Section=\"" + sect + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Section type = " + sect + " is passed to setSection().";
                throw std::invalid_argument(error_message);
            }
        }

        void setValue(const std::string& value) {
            if (current_tag != TAG::Time && current_tag != TAG::Information) {
                std::cerr << "[SIMPLE XDMF ERROR] setValue() cannot be called when current Tag is not Time and Information." << std::endl;
                return;
            }
            m_buffer += " Value=\"" + value + "\"";
        }

        void setCollectionType(const std::string& type) {
            if (current_tag != TAG::Grid) {
                std::cerr << "[SIMPLE XDMF ERROR] setCollectionType() cannot be called when current Tag is not Grid." << std::endl;
                return;
            }

            if (type == "Temporal") {
                m_buffer += " CollectionType=\"" + type + "\"";
            } else {
                std::string error_message = "[SIMPLE XDMF ERROR] Invalid Collection type = " + type + " is passed to setCollectionType().";
                throw std::invalid_argument(error_message);
            }
            
        }

        template<typename... Args>
        void setDimensions(Args&&... args) {
            std::string dimString = convertFromVariadicArgsToString(std::forward<Args>(args)...);
            m_buffer += " Dimensions=\"" + dimString + "\"";
        }

        template<typename... Args>
        void setNumberOfElements(Args&&... args) {
            std::string dimString = convertFromVariadicArgsToString(std::forward<Args>(args)...);
            m_buffer += " NumberOfElements=\"" + dimString + "\"";
        }

        // Reference Attribute management
        void setReference(const std::string& xpath) {
            m_buffer += " Reference=\"" + xpath + "\"";
        }

        void setReferenceFromName(const std::string& name) {
            auto xpath = getXpath(name);
            m_buffer += " Reference=\"" + xpath + "\"";
        }

        void addReferenceFromName(const std::string& name) {
            auto xpath = getXpath(name);
            addItem(xpath);
        }

        // helper functoins
        void begin2DStructuredGrid(const std::string& gridName, const std::string& topologyType, const int nx, const int ny) {
            beginGrid(gridName);

            beginStructuredTopology("", topologyType);
            setNumberOfElements(nx, ny);
            endStructuredTopology();
        }
        
        void end2DStructuredGrid() {
            endGrid();
        }

        template<typename T>
        void add2DGeometryOrigin(const std::string& geomName, const T origin_x, const T origin_y, const T dx, const T dy) {
            beginGeometry(geomName, "ORIGIN_DXDY");

            // Origin
            beginDataItem();
            setDimensions(2);
            setFormat("XML");
            addItem(origin_y, origin_x);
            endDataItem();

            // Strands
            beginDataItem();
            setDimensions(2);
            setFormat("XML");
            addItem(dy, dx);
            endDataItem();

            endGeometry();
        }

        void begin3DStructuredGrid(const std::string& gridName, const std::string& topologyType, const int nx, const int ny, const int nz) {
            beginGrid(gridName);

            beginStructuredTopology("", topologyType);
            setNumberOfElements(nx, ny, nz);
            endStructuredTopology();
        }
        
        void end3DStructuredGrid() {
            endGrid();
        }

        template<typename T>
        void add3DGeometryOrigin(const std::string& geomName, const T origin_x, const T origin_y, const T origin_z, const T dx, const T dy, const T dz) {
            beginGeometry(geomName, "ORIGIN_DXDYDZ");

            // Origin
            beginDataItem();
            setDimensions(3);
            setFormat("XML");
            addItem(origin_z, origin_y, origin_x);
            endDataItem();

            // Strands
            beginDataItem();
            setDimensions(3);
            setFormat("XML");
            addItem(dz, dy, dx);
            endDataItem();

            endGeometry();
        }
};

#endif