#ifndef USB_PARSER_H
#define USB_PARSER_H

#include <string>
#include <vector>
#include "csv.hpp"  // Utilize o caminho correto conforme sua estrutura de pastas

// Tipos de descritores
enum class DescriptorTypes {
    DeviceDescriptor,
    ConfigurationDescriptor,
    InterfaceDescriptor,
    EndpointDescriptor,
    StringDescriptor,
    Other   // Para descritores que não se encaixam nos casos principais
};

// Tipos de requisições
enum class RequestTypes {
    CLEAR_FEATURE,
    GET_CONFIGURATION,
    GET_DESCRIPTOR,
    GET_INTERFACE,
    GET_STATUS,
    SET_ADDRESS,
    SET_CONFIGURATION,
    SET_DESCRIPTOR,
    SET_FEATURE,
    SET_INTERFACE,
    SYNCH_FRAME,
    ControlTransfer,
    GetDeviceStatus,
    Other   // Para transações que não se encaixam nos casos principais
};

// Função que converte a string para um DescriptorTypes
DescriptorTypes getDescriptorTypes(const std::string &descriptorStr);

// Função que converte a string para um RequestTypes
RequestTypes getRequestTypes(const std::string &requestStr);

// Função auxiliar para contar os espaços iniciais (indentação)
size_t countLeadingSpaces(const std::string &str);

// Estrutura para armazenar uma transação
struct Transaction {
    DescriptorTypes descriptorType = DescriptorTypes::Other;
    RequestTypes requestType = RequestTypes::Other;
    std::vector<csv::CSVRow> rows;
};

#endif // USB_PARSER_H
