# Design Decisions

## Goal:
*recreate fallout 2 with modern systems and potential UI and gameplay improvements*


---
### custom files opened via specific libraries in src/file_libs/:  
 - files will have different endianness relative to system memory and thus must be converted during parsing  
 - do not assume the user will be x86_64 little endian  
 - all external file data should be passed through endian reversing functions, however same-endian transformation should be removed at compile-time  
 - all files should contain a common file info struct for parsing and metadata purposes  
