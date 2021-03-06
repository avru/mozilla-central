/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * The origin of this IDL file is
 * https://dvcs.w3.org/hg/audio/raw-file/tip/webaudio/specification.html
 *
 * Copyright © 2012 W3C® (MIT, ERCIM, Keio), All Rights Reserved. W3C
 * liability, trademark and document use rules apply.
 */

[PrefControlled]
interface AudioNode {

    void connect(AudioNode destination, optional unsigned long output = 0, optional unsigned long input = 0);

    //void connect(AudioParam destination, optional unsigned long output = 0);

    void disconnect(optional unsigned long output = 0);

    readonly attribute AudioContext context;
    //readonly attribute unsigned long numberOfInputs;
    //readonly attribute unsigned long numberOfOutputs;

};

