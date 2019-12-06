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
            'cmmcRemote_setup',
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