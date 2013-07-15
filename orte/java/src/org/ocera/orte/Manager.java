package org.ocera.orte;

import org.ocera.orte.types.*;;

public class Manager {
	
	private DomainProp dprops;
	private DomainMgr dmgr;
	
	public Manager(String[] mgrs) {
		dprops = DomainProp.defaultPropsCreate();
		dprops.setProps(mgrs);
		
		dmgr = new DomainMgr(ORTEConstant.ORTE_DEFAULT_DOMAIN,
							 dprops,
							 null,
							 false);
	}
}
