{
  "targets": [
    {
      "target_name": "xenstat",
      "sources": [ "src/node_xenstat.cc", "src/domain.cc", "src/node.cc" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")",
        "xen"
      ],
      "libraries": [
        "-lxenstat",
        "-L<!(/usr/lib/xen-common/bin/xen-dir)/lib",
        "-Wl,-rpath=<!(/usr/lib/xen-common/bin/xen-dir)/lib"
      ],
    }
  ]
}
