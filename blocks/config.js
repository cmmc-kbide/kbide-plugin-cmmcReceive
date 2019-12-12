module.exports = [
    {
        name: "CMMC Remote",
        blocks: [
            {
                xml: `<sep gap="32"></sep><label text="Plugin for CMMC Remote Application" web-class="headline"></label>`
            },
            {
                xml: `<sep gap="32"></sep><label text="Setup" web-class="headline"></label>`
            },
            {
              xml:
              `<block type="cmmcRemote_setup">
                <value name="NAME">
                  <block type="basic_string">
                    <field name="VALUE">CMMC001</field>
                  </block>
                </value>
                <value name="PSK">
                  <block type="basic_string">
                    <field name="VALUE">hellokbide</field>
                  </block>
                </value>
              </block>`
            },
            {
                xml: `<sep gap="32"></sep><label text="Loop" web-class="headline"></label>`
            },
            'cmmcRemote_loop',
            {
                xml: `<sep gap="32"></sep><label text="Get Data" web-class="headline"></label>`
            },
            "cmmcRemote_getRoll",
            "cmmcRemote_getPitch",
            "cmmcRemote_getThrottle",
            "cmmcRemote_getYaw"
        ]
    }
];