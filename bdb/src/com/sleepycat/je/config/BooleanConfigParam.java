/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2002,2006 Oracle.  All rights reserved.
 *
 * $Id: BooleanConfigParam.java,v 1.25 2006/11/06 20:36:56 linda Exp $
 */

package com.sleepycat.je.config;

/**
 * A JE configuration parameter with an boolean value.
 */
public class BooleanConfigParam extends ConfigParam {
    
    private static final String DEBUG_NAME =
        BooleanConfigParam.class.getName();

    /**
     * Set a boolean parameter w/default.
     * @param configName
     * @param defaultValue
     * @param forReplication TODO
     */
    BooleanConfigParam(String configName,
                       boolean defaultValue,
                       boolean mutable,
                       boolean forReplication, 
                       String description) {
        // defaultValue must not be null
        super(configName,
	      Boolean.valueOf(defaultValue).toString(),
              mutable,
              forReplication, 
              description);
    }

    /**
     * Make sure that value is a valid string for booleans.
     */
    public void validateValue(String value)
        throws IllegalArgumentException {

        if (!value.trim().equalsIgnoreCase(Boolean.FALSE.toString()) &&
            !value.trim().equalsIgnoreCase(Boolean.TRUE.toString())) {
            throw new IllegalArgumentException
		(DEBUG_NAME + ": " +  value + " not valid boolean " + name);
        }
    }
}