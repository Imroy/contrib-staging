/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2002,2006 Oracle.  All rights reserved.
 *
 * $Id: RecoveryException.java,v 1.14 2006/10/30 21:14:22 bostic Exp $
 */

package com.sleepycat.je.recovery;

import com.sleepycat.je.RunRecoveryException;
import com.sleepycat.je.dbi.EnvironmentImpl;

/**
 * Recovery related exceptions
 */
public class RecoveryException extends RunRecoveryException {

    public RecoveryException(EnvironmentImpl env,
                             String message,
                             Throwable t) {
	super(env, message, t);
    }
    public RecoveryException(EnvironmentImpl env,
                             String message) {
	super(env, message);
    }
}