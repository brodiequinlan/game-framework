#pragma once

#include <cctype>
#include <sstream>
#include <map>
#include <string>


namespace bq{

    class json;
struct json_array{
        std::map<int, std::string> m_strings;
        std::map<int, bq::json> m_objects;

        void print(){
            for(auto& ele : m_strings){
                bq::logger::info(ele.second);
            }
        }
        int get_i(int index){
            return std::stoi(m_strings[index]);
        }
        bool get_b(int index){
            #ifdef DEBUG
                if(! (m_strings.count(index) > 0)) bq::logger::critical(index + " does not exist in json!");
            #endif
            std::string s = m_strings[index];
            if(s == "true"){
                return true;
            }
            else{
                return false;
            }
        }
        bq::json get_o(int index);
        std::string get_s(int index){
            return m_strings[index];
        }
    };
    

    class json{
        std::map<std::string, std::string> m_data;
        
public:
        std::map<std::string, bq::json_array> m_arrays;
        std::map<std::string, bq::json> m_subObjects;   
        void add(const std::string& key, const std::string& value) {
            m_data[key] = value;
        }
        std::string get_s(const std::string& key){
            #ifdef DEBUG
                if(! (m_data.count(key) > 0)) bq::logger::critical(key + " does not exist in json!");
            #endif

            return m_data[key];
        }
        int get_i(const std::string& key){
            #ifdef DEBUG
                if(! (m_data.count(key) > 0)) bq::logger::critical(key + " does not exist in json!");
            #endif
            return std::stoi(m_data[key]);
        }
        bool get_b(const std::string& key){
            #ifdef DEBUG
                if(! (m_data.count(key) > 0)) bq::logger::critical(key + " does not exist in json!");
            #endif
            std::string s = m_data[key];
            if(s == "true"){
                return true;
            }
            else{
                return false;
            }
        }
        bq::json get_o(const std::string& key){
            #ifdef DEBUG
                if(! (m_subObjects.count(key) > 0)) bq::logger::critical(key + " does not exist in json!");
            #endif
            return m_subObjects[key];
        }
        bq::json_array get_a(const std::string& key){
            #ifdef DEBUG
                if(! (m_arrays.count(key) > 0)) bq::logger::critical(key + " does not exist in json!");
            #endif
            return m_arrays[key];
        }
        void print(){
            for(auto& ele : m_data){
                bq::logger::info("Key: " + ele.first);
                bq::logger::info("Value: " + ele.second);
            }
            for(auto element : m_arrays){
                bq::logger::info("Key: " + element.first);
                element.second.print();
            }
            
        }
        static bq::json parse(const std::string& json_text){
            bq::json j;

            bool objectOpen = false;

            bool writingName = false;
            bool writingValue = false;
            std::string name = "";
            std::string value = "";

            bool first_valid_value_written = false;
            bool isString = false;
            bool potential_string_end = false;
            bool building_sub_object = false;

            bool building_array = false;
            bq::json_array ja;

            objectOpen = false;

            writingName = false;
            writingValue = false;
            name = "";
            value = "";

            first_valid_value_written = false;
            isString = false;
            potential_string_end = false;
            building_sub_object = false;

            int array_index = 0;

            bq::json sub_obj;
            for(char i : json_text){

                if(writingName){
                    if(i != '\"')
                    {
                        name += i;
                        continue;
                    }

                }
                if(writingValue){
                    if(!first_valid_value_written){
                        if(i == ' '){
                            continue;
                        }
                        else if(i == '\"' || i == '"'){
                            isString = true;
                            first_valid_value_written = true;
                            continue;
                        }
                        else if(i == 't'){
                            value += i;
                            first_valid_value_written = true;
                            continue;
                        }
                        else if(i == 'f'){
                            value += i;
                            first_valid_value_written = true;
                            continue;
                        }
                        else if( isdigit(i) ){
                            value += i;
                            first_valid_value_written = true;
                            continue;
                        }else if(i == '{'){
                            building_sub_object = true;
                            first_valid_value_written = true;
                            value += '{';
                            continue;
                        }
                        else if( i == '['){
                            building_array = true;
                        }
                        else if ( i == ']'){
                            building_array = false;
                            j.m_arrays[name] = ja;
                            array_index = 0;
                            name = "";
                            value = "";
                            writingValue = false;
                            ja = bq::json_array();
                            continue;
                        }
                    }else{
                        if(isString){
                            if(i == '"' || i == '\"'){
                                if(building_array){
                                    ja.m_strings[array_index] = value;
                                    array_index++;
                                }
                                else{
                                    j.add(name, value); 
                                    writingValue = false;
                                    name = "";
                                    
                                }
                                value = ""; 
                                objectOpen = false;
                                writingName = false;
                                first_valid_value_written = false;
                                isString = false;
                                potential_string_end = false;
                                building_sub_object = false;
                                continue;
                            }
                            else{
                                value += i;
                                continue;
                            }
                        }
                        else if(building_sub_object){
                            value += i;
                            if(i == '}'){
                                sub_obj = parse(value);
                                if(building_array){
                                    ja.m_objects[array_index] = sub_obj;
                                    array_index++;

                                }
                                else{
                                    
                                    j.m_subObjects[name] = sub_obj;
                                    writingValue = false;
                                    name = "";
                                    
                                }
                                
                                sub_obj = bq::json();
                                value = ""; 
                                objectOpen = false;
                                writingName = false;
                                first_valid_value_written = false;
                                isString = false;
                                potential_string_end = false;
                                building_sub_object = false;


                                
                            }
                            continue;
                        }
                        else{
                            if(i == ',' || i == '{' || i == '}' || i==' '){
                                if(building_array){
                                    ja.m_strings[array_index] = value;
                                    array_index++;
                                }
                                    
                                else{
                                    j.add(name, value); 
                                    writingValue = false;
                                    name = "";
                                }
                                value = ""; 
                                objectOpen = false;
                                writingName = false;
                                first_valid_value_written = false;
                                isString = false;
                                potential_string_end = false;
                                building_sub_object = false;
                                continue;
                            }
                            else{
                                value += i;
                                if(building_array && std::isdigit( i ))
                                first_valid_value_written = false;
                                continue;
                            }
                        }
                    }
                }
                
                if(i == '{'){
                    objectOpen = true;
                }
                if(i == '}'){
                    objectOpen = false;
                }
                if(i == ':'){
                    writingValue = true;
                }
                if(i == '"'){
                    if(!writingName && !writingValue){
                        writingName = true;
                    }
                    else{
                        writingName = false;
                    }
                }

            }
            if(name != "" && value != ""){
                j.add(name, value);
            }
            return j;
        };
    };
};