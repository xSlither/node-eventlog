{ 
  "targets": [ 
    { 
      "target_name": "node-eventlog",

	  "win_delay_load_hook": "true",

	  "include_dirs": [
		"<!@(node -p \"require('node-addon-api').include\")"
	  ],

	  "link_settings": {
		"library_dirs" :[
		],

		"libraries": [ 
		],	  
	  },

	  "cflags!": [ "-fPIC", "-fno-exceptions", "-std=c++17", "-stdlib=libc++" ],
	  "cflags_cpp!": [ "-fPIC", "-fno-exceptions", "-std=c++17", "-stdlib=libc++" ],
	  "cflags_cc!": [ "-fPIC", "-fno-exceptions", "-std=c++17", "-stdlib=libc++" ],

	  "conditions": [
		  ["OS=='win'", {
			  "sources": [
				"./src/logger.hpp",
				"./src/logger.cpp",
				"./src/main.cpp"
			  ]
		  }]
	  ],

	  'defines': [
		  'NAPI_DISABLE_CPP_EXCEPTIONS', 
		  '_HAS_EXCEPTIONS'
	   ],

	   "msvs_settings": {
		   "VCCLCompilerTool": {
			   "ExceptionHandling": 1,
			   "AdditionalOptions": [ '-std:c++17', '/MP' ]
		   }
	   }
    },
	{
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
	}
  ]
}