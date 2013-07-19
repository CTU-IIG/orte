package org.ocera.orte;

import org.ocera.orte.types.*;;

public class Manager {
	
	private DomainMgr dmgr;
	
	public Manager(String[] mgrs) {
		DomainProp dprops = DomainProp.defaultPropsCreate();
		dprops.setProps(mgrs);
		
		dmgr = new DomainMgr(ORTEConstant.ORTE_DEFAULT_DOMAIN,
							 dprops,
							 null,
							 false);
	}

        public boolean destroy() {
                return dmgr.destroy())
        }
}
