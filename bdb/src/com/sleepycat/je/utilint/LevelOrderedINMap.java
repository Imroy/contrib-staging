/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2002,2006 Oracle.  All rights reserved.
 *
 * $Id: LevelOrderedINMap.java,v 1.7 2006/10/30 21:14:29 bostic Exp $
 */

package com.sleepycat.je.utilint;

import java.util.HashSet;
import java.util.Set;
import java.util.TreeMap;

import com.sleepycat.je.tree.IN;

/**
 * A level ordered map holds collection of INs, sorted by level. The map is
 * keyed by level and each datum is a set of INs belonging to that level.
 */
public class LevelOrderedINMap extends TreeMap {
    
    public void putIN(IN in) {
        Integer level = new Integer(in.getLevel());
        Set inSet = (Set) get(level);
        if (inSet == null) {
            inSet = new HashSet();
            put(level, inSet);
        }
        inSet.add(in);
    }
}