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
        "-L/usr/lib/xen-4.4/lib", "-L/usr/lib/xen-4.3/lib", "-L/usr/lib/xen-4.2/lib",
        "-Wl,-rpath=/usr/lib/xen-4.4/lib,-rpath=/usr/lib/xen-4.3/lib,-rpath=/usr/lib/xen-4.2/lib"
      ],
    }
  ]
}
