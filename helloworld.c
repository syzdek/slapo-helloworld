/*
 *  OpenLDAP Hello World Overlay
 *  Copyright (c) 2023 David M. Syzdek <david@syzdek.net>
 *  All rights reserved.
 *
 *  Dominus vobiscum. Et cum spiritu tuo.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted only as authorized by the OpenLDAP
 *  Public License.
 *
 *  A copy of this license is available in the file LICENSE in the
 *  top-level directory of the distribution or, alternatively, at
 *  <http://www.OpenLDAP.org/license.html>.
 */
#include "portable.h"
#ifdef SLAPD_OVER_HELLOWORLD

///////////////
//           //
//  Headers  //
//           //
///////////////
#ifndef SLAPD_OVER_HELLOWORLD
#	pragma mark - Headers
#endif

#include <ldap.h>
#include "slap.h"
#include "slap-config.h"
#ifdef SLAPD_MODULES
#	include <ltdl.h>
#endif


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifndef SLAPD_OVER_HELLOWORLD
#	pragma mark - Definitions
#endif


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////
#ifndef SLAPD_OVER_HELLOWORLD
#	pragma mark - Datatypes
#endif

typedef struct helloworld_cnt_t
{
	int							c_total;
	int							c_grandparent;
	int							c_parent;
	int							c_sibling;
	int							c_spouse;
	int							c_child;
	int							c_grandchild;
	int							c_godparent;
	int							c_godchild;
} helloworld_cnt_t;


typedef struct helloworld_at_t
{
	char *						def;
	AttributeDescription **		ad;
} helloworld_at_t;


typedef struct helloworld_oc_t
{
	char *						def;
	ObjectClass **				oc;
} helloworld_oc_t;


typedef struct helloworld_t
{
	int							hw_count_family;
} helloworld_t;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifndef SLAPD_OVER_HELLOWORLD
#	pragma mark - Prototypes
#endif

//static int
//hello_cfg_gen(
//		ConfigArgs *				c );


static int
hello_count_entry_attr(
		Entry *						e,
		int *						iptr,
		AttributeDescription *		ad );


static int
hello_db_destroy(
		BackendDB *					be,
		ConfigReply *				cr );


static int
hello_db_init(
		BackendDB *					be,
		ConfigReply *				cr );


//static int
//hello_op_add(
//		Operation *					op,
//		SlapReply *					rs );


static int
hello_get_entry_counts(
		Entry *						e,
		helloworld_cnt_t *			cnt );


//static int
//hello_op_modify(
//		Operation *					op,
//		SlapReply *					rs );


//static int
//hello_op_search(
//		Operation *					op,
//		SlapReply *					rs );


extern int
helloworld_initialize(
		void );


extern int
init_module(
		int							argc,
		char *						argv[] );


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#ifndef SLAPD_OVER_HELLOWORLD
#	pragma mark - Variables
#endif

static slap_overinst				helloworld;

// operational attribute descriptions
static AttributeDescription *		ad_helloBestFriend			= NULL;
static AttributeDescription *		ad_helloFamilySize			= NULL;

// user attribute descriptions
static AttributeDescription *		ad_helloGrandparent			= NULL;
static AttributeDescription *		ad_helloParent				= NULL;
static AttributeDescription *		ad_helloSibling				= NULL;
static AttributeDescription *		ad_helloSpouse				= NULL;
static AttributeDescription *		ad_helloChild				= NULL;
static AttributeDescription *		ad_helloGrandchild			= NULL;
static AttributeDescription *		ad_helloGodparent			= NULL;
static AttributeDescription *		ad_helloGodchild			= NULL;

// user objectClasses
static ObjectClass *				oc_helloFamily				= NULL;


// operational and user attribute definitions
static helloworld_at_t hello_ats[] =
{
	// operational attribute definitions
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.1.1"
				" NAME ( 'helloBestFriend' )"
				" DESC 'The subentry of the users best friend'"
				" EQUALITY distinguishedNameMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.12"
				" SINGLE-VALUE "
				" USAGE directoryOperation )",
		.ad		= &ad_helloBestFriend
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.1.2"
				" NAME ( 'helloFamilySize' )"
				" DESC 'The number of individuals in the users family'"
				" EQUALITY integerMatch"
				" ORDERING integerOrderingMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.27"
				" SINGLE-VALUE "
				" NO-USER-MODIFICATION"
				" USAGE directoryOperation )",
		.ad		= &ad_helloFamilySize
	},
	// user attribute definitions
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.1"
				" NAME ( 'helloGrandparent' )"
				" DESC 'The name of one of the users grandparents'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloGrandparent
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.2"
				" NAME ( 'helloParent' )"
				" DESC 'The name of one of the users parents'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloParent
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.3"
				" NAME ( 'helloSibling' )"
				" DESC 'The name of one of the users siblings'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloSibling
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.4"
				" NAME ( 'helloSpouse' )"
				" DESC 'The name of the users spouse'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SINGLE-VALUE "
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloSpouse
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.5"
				" NAME ( 'helloChild' )"
				" DESC 'The name of one of the users children'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloChild
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.6"
				" NAME ( 'helloGrandchild' )"
				" DESC 'The name of one of the users grandchildren'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloGrandchild
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.7"
				" NAME ( 'helloGodparent' )"
				" DESC 'The name of one of the users godparents'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloGodparent
	},
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.2.8"
				" NAME ( 'helloGodchild' )"
				" DESC 'The name of one of the users godchildren'"
				" EQUALITY caseIgnoreMatch"
				" ORDERING caseIgnoreOrderingMatch"
				" SUBSTR caseIgnoreSubstringsMatch"
				" SYNTAX 1.3.6.1.4.1.1466.115.121.1.15 )",
		.ad		= &ad_helloGodchild
	},
	{
		.def	= NULL,
		.ad		= NULL
	}
};


// user objectClass definitions
static helloworld_oc_t hello_ocs[] =
{
	{	.def	= "( 1.3.6.1.4.1.27893.4.3.3.1"
				" NAME 'helloFamily'"
				" DESC 'Individuals of a family'"
				" SUP top"
				" AUXILIARY"
				" MAY ( helloGrandparent $ helloParent $ helloSibling $"
					" helloSpouse $ helloChild $ helloGrandchild $"
					" helloGodparent $ helloGodchild ) )",
		.oc		= &oc_helloFamily
    },
	{
		.def	= NULL,
		.oc		= NULL
	}
};


// configuration options and attribute definitions
static ConfigTable hello_cfg_ats[] =
{
	{	.name		= "hello_count_family",
		.what		= "on|off",
		.min_args	= 2,
		.max_args	= 2,
		.length		= 0,
		.arg_type	= ARG_ON_OFF|ARG_OFFSET,
		.arg_item	= (void *)offsetof(helloworld_t,hw_count_family),
		.attribute	= "( 1.3.6.1.4.1.27893.4.3.4.1"
					" NAME 'olcHelloCountFamily'"
					" DESC 'Count members of the users family'"
					" EQUALITY booleanMatch"
					" SYNTAX OMsBoolean"
					" SINGLE-VALUE )"
	},
	{	.name		= NULL,
		.what		= NULL,
		.min_args	= 0,
		.max_args	= 0,
		.length		= 0,
		.arg_type	= ARG_IGNORED,
		.arg_item	= NULL,
		.attribute	= NULL
	}
};


// configuration objectclass definitions
static ConfigOCs hello_cfg_ocs[] =
{
	{	.co_def		= "( 1.3.6.1.4.1.27893.4.3.5.1"
					" NAME 'olcHelloWorldConfig'"
					" DESC 'Hello World configuration'"
					" SUP olcOverlayConfig"
					" MAY ( olcHelloCountFamily ) )",
		.co_type	= Cft_Overlay,
		.co_table	= hello_cfg_ats
	},
	{	.co_def		= NULL,
		.co_type	= 0,
		.co_table	= NULL
	}
};


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifndef SLAPD_OVER_HELLOWORLD
#	pragma mark - Functions
#endif

int
hello_count_entry_attr(
		Entry *						e,
		int *						iptr,
		AttributeDescription *		ad )
{
	Attribute * a;
	if ((a = attr_find(e->e_attrs, ad)) == NULL)
		return(0);
	*iptr += (int)a->a_numvals;
	return(0);
}


int
hello_db_destroy(
		BackendDB *					be,
		ConfigReply *				cr )
{
	slap_overinst *			on;
	helloworld_t *			hw;

	on						= (slap_overinst *) be->bd_info;
	hw						= on->on_bi.bi_private;
	on->on_bi.bi_private	= NULL;

	memset(hw, 0, sizeof(helloworld_t));
	ch_free(hw);

	if ((cr))
		return(0);
	return(0);
}


int
hello_db_init(
		BackendDB *					be,
		ConfigReply *				cr )
{
	slap_overinst *			on;
	helloworld_t *			hw;

	if ((SLAP_ISGLOBALOVERLAY(be)))
	{
		// do not allow slapo-pwdshadow to be global
		if ((cr))
		{
			snprintf( cr->msg, sizeof(cr->msg), "slapo-helloworld cannot be global" );
			Debug( LDAP_DEBUG_ANY, "%s\n", cr->msg );
		};
		return(1);
	};

	// allocate memory for database instance configuration
	on						= (slap_overinst *)be->bd_info;
	on->on_bi.bi_private	= ch_calloc(sizeof(helloworld_t), 1);
	hw						= on->on_bi.bi_private;

	// set default values
	hw->hw_count_family		= 0;

	return(0);
}


int
hello_get_entry_counts(
		Entry *						e,
		helloworld_cnt_t *			cnt )
{
	hello_count_entry_attr(e, &cnt->c_total,		ad_helloFamilySize);
	hello_count_entry_attr(e, &cnt->c_grandparent,	ad_helloGrandparent);
	hello_count_entry_attr(e, &cnt->c_parent,		ad_helloParent);
	hello_count_entry_attr(e, &cnt->c_sibling,		ad_helloSibling);
	hello_count_entry_attr(e, &cnt->c_spouse,		ad_helloSpouse);
	hello_count_entry_attr(e, &cnt->c_child,		ad_helloChild);
	hello_count_entry_attr(e, &cnt->c_grandchild,	ad_helloGrandchild);
	hello_count_entry_attr(e, &cnt->c_godparent,	ad_helloGodparent);
	hello_count_entry_attr(e, &cnt->c_godchild,		ad_helloGodchild);
	return(0);
}


int
helloworld_initialize( void )
{
	int		i;
	int		code;

	// register user and operation attributeTypes
	for(i = 0; ((hello_ats[i].def)); i++)
	{
		if ((code = register_at(hello_ats[i].def, hello_ats[i].ad, 0)) != 0)
		{
			Debug(LDAP_DEBUG_ANY, "helloworld_initialize: register_at failed\n");
			Debug(LDAP_DEBUG_ANY, "helloworld_initialize: %s\n", hello_ats[i].def);
			return(code);
		};
	};

	// register user objectClasses
	for (i = 0; ((hello_ocs[i].def)); i++)
	{
		if ((code = register_oc(hello_ocs[i].def, hello_ocs[i].oc, 0)) != 0)
		{
			Debug(LDAP_DEBUG_ANY, "helloworld_initialize: register_oc failed\n");
			Debug(LDAP_DEBUG_ANY, "helloworld_initialize: %s\n", hello_ocs[i].def);
			return(code);
		};
	};

	// register configuration options/attributes
	if ((code = config_register_schema(hello_cfg_ats, hello_cfg_ocs)) != 0)
	{
		Debug(LDAP_DEBUG_ANY, "helloworld_initialize: config_register_schema failed\n");
		return(code);
	};

	helloworld.on_bi.bi_type			= "helloworld";
	helloworld.on_bi.bi_flags			= SLAPO_BFLAG_SINGLE;

	helloworld.on_bi.bi_db_init			= hello_db_init;
	//helloworld.on_bi.bi_db_open		= hello_db_open;
	//helloworld.on_bi.bi_db_close		= hello_db_close;
	helloworld.on_bi.bi_db_destroy		= hello_db_destroy;

	//helloworld.on_bi.bi_op_add		= hello_op_add;
	//helloworld.on_bi.bi_op_bind		= hello_op_bind;
	//helloworld.on_bi.bi_op_compare	= hello_op_compare;
	//helloworld.on_bi.bi_op_delete		= hello_op_delete;
	//helloworld.on_bi.bi_op_modify		= hello_op_modify;
	//helloworld.on_bi.bi_op_search		= hello_op_search;

	helloworld.on_bi.bi_cf_ocs			= hello_cfg_ocs;

	return(overlay_register(&helloworld));
}


#if SLAPD_OVER_HELLOWORLD == SLAPD_MOD_DYNAMIC
int
init_module(
		int							argc,
		char *						argv[] )
{
	if ((argc))
		return( helloworld_initialize() );
	if ((argv))
		return( helloworld_initialize() );
	return( helloworld_initialize() );
}
#endif

#endif
/* end of source file */
