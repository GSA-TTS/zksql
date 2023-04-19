#ifndef _TPCH_QUERIES_H
#define _TPCH_QUERIES_H

#include <emp-tool/utils/constants.h>
#include <map>


using namespace std;


namespace vaultdb {

    std::map<int, std::string> tpch_queries = {
            {1, "select \n"
                "  l_returnflag, \n"
                "  l_linestatus, \n"
                "  sum(l_quantity)::BIGINT as sum_qty, \n"
                "  sum(l_extendedprice)::BIGINT as sum_base_price, \n"
                "  sum(l_extendedprice * (100 - l_discount))::BIGINT as sum_disc_price, \n"
                "  sum(l_extendedprice * (100 - l_discount) * (100 + l_tax))::BIGINT as sum_charge, \n"
                "  FLOOR(avg(l_quantity))::BIGINT as avg_qty, \n"
                "  FLOOR(avg(l_extendedprice))::BIGINT as avg_price, \n"
                "  FLOOR(avg(l_discount))::BIGINT as avg_disc, \n"
                "  count(*)::BIGINT as count_order \n"
                "from \n"
                "  lineitem \n"
                " where \n"
                "  l_shipdate <= date '1998-08-03' \n"
                "group by \n"
                "  l_returnflag, \n"
                "  l_linestatus \n"
                " \n"
                "order by \n"
                "  l_returnflag, \n"
                "  l_linestatus"},

            {3, "select\n"
                "  l.l_orderkey,\n"
                "  sum(l.l_extendedprice * (100 - l.l_discount))::BIGINT as revenue,\n"
                "  o.o_orderdate,\n"
                "  o.o_shippriority\n"
                " from\n"
                "  customer c JOIN orders o ON  c.c_custkey = o.o_custkey\n"
                "     JOIN lineitem l ON l.l_orderkey = o.o_orderkey\n"
                " where\n"
                "  c.c_mktsegment = 'HOUSEHOLD'\n"
                "  and o.o_orderdate < date '1995-03-25'\n"
                "  and l.l_shipdate > date '1995-03-25'\n"
                "\n"
                " group by\n"
                "  l.l_orderkey,\n"
                "  o.o_orderdate,\n"
                "  o.o_shippriority\n"
                " order by\n"
                "  revenue desc,\n"
                "  o.o_orderdate\n"
                " limit 10"},
            {5, "select\n"
                "  n.n_name,\n"
                "  sum(l.l_extendedprice * (100 - l.l_discount))::BIGINT as revenue\n"
                " from\n"
                "  customer c JOIN orders o ON c.c_custkey = o.o_custkey\n"
                "     JOIN lineitem l ON l.l_orderkey = o.o_orderkey\n"
                "     JOIN supplier s ON l.l_suppkey = s.s_suppkey\n"
                "     JOIN nation n ON s.s_nationkey = n.n_nationkey\n"
                "     JOIN region r ON n.n_regionkey = r.r_regionkey\n"
                "where\n"
                "   c.c_nationkey = s.s_nationkey\n"
                "  and r.r_name = 'EUROPE'\n"
                "  and o.o_orderdate >= date '1997-01-01'\n"
                "  and o.o_orderdate < date '1998-01-01'\n"
                " group by\n"
                "  n.n_name\n"
                " order by\n"
                "  revenue desc"},
            {8,     "select\n"
                    "  o_year,\n"
                    "  (sum(case\n"
                    "    when n_nation = 'EGYPT' then volume\n"
                    "    else 0\n"
                    "  end) / sum(volume))::BIGINT as mkt_share\n"
                    "from\n"
                    "  (\n"
                    "    select\n"
                    "      extract(year from o.o_orderdate)::INT as o_year,\n"
                    "      l.l_extendedprice * (100 - l.l_discount)::BIGINT as volume,\n"
                    "      n2.n_name as n_nation\n"
                    "    from\n"
                    "      part p,\n"
                    "      supplier s,\n"
                    "      lineitem l,\n"
                    "      orders o,\n"
                    "      customer c,\n"
                    "      nation n1,\n"
                    "      nation n2,\n"
                    "      region r\n"
                    "    where\n"
                    "      p.p_partkey = l.l_partkey\n"
                    "      and s.s_suppkey = l.l_suppkey\n"
                    "      and l.l_orderkey = o.o_orderkey\n"
                    "      and o.o_custkey = c.c_custkey\n"
                    "      and c.c_nationkey = n1.n_nationkey\n"
                    "      and n1.n_regionkey = r.r_regionkey\n"
                    "      and r.r_name = 'MIDDLE EAST'\n"
                    "      and s.s_nationkey = n2.n_nationkey\n"
                    "      and o.o_orderdate between date '1995-01-01' and date '1996-12-31'\n"
                    "      and p.p_type = 'PROMO BRUSHED COPPER'\n"
                    "  ) as all_nations\n"
                    "group by\n"
                    "  o_year\n"
                    "order by\n"
                    "  o_year"},
            {9, "WITH order_years AS (\n"
                "     SELECT extract(year from o_orderdate)::INT as o_year, o.o_orderkey\n"
                "     FROM orders o),\n"
                "     yellow_parts AS (\n"
                "         SELECT p_partkey\n"
                "	 FROM part),\n"
                //"	 WHERE p_name like '%yellow%'),\n"
                "     profit AS (\n"
                "         select\n"
                "    	   n_name,\n"
                "     	    o_year,\n"
                "      	    (l.l_extendedprice * (100 - l.l_discount) - ps.ps_supplycost * l.l_quantity)::BIGINT as amount\n"
                "    	  from\n"
                "    	    yellow_parts p,\n"
                "      	    supplier s,\n"
                "      	    lineitem l,\n"
                "      	    partsupp ps,\n"
                "      	    order_years o,\n"
                "      	    nation n\n"
                "    where\n"
                "      s.s_suppkey = l.l_suppkey\n"
                "      and ps.ps_suppkey = l.l_suppkey\n"
                "      and ps.ps_partkey = l.l_partkey\n"
                "      and p.p_partkey = l.l_partkey\n"
                "      and o.o_orderkey = l.l_orderkey\n"
                "      and s.s_nationkey = n.n_nationkey)\n"
                " select\n"
                "  n_name,\n"
                "  o_year,\n"
                "  sum(amount)::BIGINT as sum_profit\n"
                " from profit\n"
                " group by\n"
                "  n_name,\n"
                "  o_year\n"
                " order by\n"
                "  n_name,\n"
                "  o_year desc\n"},
            {18, "WITH high_quantity AS (\n"
                 " SELECT l_orderkey\n"
                 " FROM lineitem\n"
                 " GROUP BY l_orderkey\n"
                 " HAVING SUM(l_quantity)::BIGINT > 313) \n"
                 " select c.c_name,\n"
                 "    c.c_custkey,\n"
                 "   o.o_orderkey,\n"
                 "   o.o_orderdate,\n"
                 "   o.o_totalprice,\n"
                 "   sum(l.l_quantity)::BIGINT\n"
                 " from\n"
                 "   customer c,\n"
                 "   orders o,\n"
                 "   lineitem l\n"
                 " where\n"
                 "   o.o_orderkey in (\n"
                 "     select * FROM high_quantity)\n"
                 "   and c.c_custkey = o.o_custkey\n"
                 "   and o.o_orderkey = l.l_orderkey\n"
                 " group by\n"
                 "   c.c_name,\n"
                 "   c.c_custkey,\n"
                 "   o.o_orderkey,\n"
                 "   o.o_orderdate,\n"
                 "   o.o_totalprice\n"
                 " order by\n"
                 "   o.o_totalprice desc,\n"
                 "   o.o_orderdate\n"
                 " limit 100\n"}
    };



    std::map<int, std::string> truncated_tpch_queries = {
            {1, "select \n"
                "  l_returnflag, \n"
                "  l_linestatus, \n"
                "  sum(l_quantity) as sum_qty, \n"
                "  sum(l_extendedprice) as sum_base_price, \n"
                "  sum(l_extendedprice * (1 - l_discount)) as sum_disc_price, \n"
                "  sum(l_extendedprice * (1 - l_discount) * (1 + l_tax)) as sum_charge, \n"
                "  avg(l_quantity) as avg_qty, \n"
                "  avg(l_extendedprice) as avg_price, \n"
                "  avg(l_discount) as avg_disc, \n"
                "  count(*) as count_order \n"
                "from \n"
                " (SELECT * FROM  lineitem ORDER BY l_orderkey, l_linenumber LIMIT $LIMIT) t1 \n"
                " where \n"
                "  l_shipdate <= date '1998-08-03' \n"
                "group by \n"
                "  l_returnflag, \n"
                "  l_linestatus \n"
                " \n"
                "order by \n"
                "  l_returnflag, \n"
                "  l_linestatus"},

            {3, "select\n"
                "  l.l_orderkey,\n"
                "  sum(l.l_extendedprice * (1 - l.l_discount)) as revenue,\n"
                "  o.o_orderdate,\n"
                "  o.o_shippriority\n"
                " from\n"
                "   (SELECT * FROM  customer ORDER BY c_custkey LIMIT $LIMIT)  c "
                "  JOIN  (SELECT * FROM  orders ORDER BY o_orderkey LIMIT $LIMIT)  o ON  c.c_custkey = o.o_custkey\n"
                "     JOIN (SELECT * FROM  lineitem ORDER BY l_orderkey, l_linenumber LIMIT $LIMIT) l ON l.l_orderkey = o.o_orderkey\n"
                " where\n"
                "  c.c_mktsegment = 'HOUSEHOLD'\n"
                "  and o.o_orderdate < date '1995-03-25'\n"
                "  and l.l_shipdate > date '1995-03-25'\n"
                "\n"
                " group by\n"
                "  l.l_orderkey,\n"
                "  o.o_orderdate,\n"
                "  o.o_shippriority\n"
                " order by\n"
                "  revenue desc,\n"
                "  o.o_orderdate\n"
                " limit 10"},
            {5, "select\n"
                "  n.n_name,\n"
                "  sum(l.l_extendedprice * (1 - l.l_discount)) as revenue\n"
                " from\n"
                "   (SELECT * FROM  customer ORDER BY c_custkey LIMIT $LIMIT)  c JOIN  (SELECT * FROM  orders ORDER BY o_orderkey LIMIT $LIMIT) o ON c.c_custkey = o.o_custkey\n"
                "     JOIN (SELECT * FROM  lineitem ORDER BY l_orderkey, l_linenumber LIMIT $LIMIT) l ON l.l_orderkey = o.o_orderkey\n"
                "     JOIN  (SELECT * FROM  supplier ORDER BY s_suppkey LIMIT $LIMIT)  s ON l.l_suppkey = s.s_suppkey\n"
                "     JOIN nation n ON s.s_nationkey = n.n_nationkey\n"
                "     JOIN region r ON n.n_regionkey = r.r_regionkey\n"
                "where\n"
                "   c.c_nationkey = s.s_nationkey\n"
                "  and r.r_name = 'EUROPE'\n"
                "  and o.o_orderdate >= date '1997-01-01'\n"
                "  and o.o_orderdate < date '1998-01-01'\n"
                " group by\n"
                "  n.n_name\n"
                " order by\n"
                "  revenue desc"},
            {8, "WITH  supplier_nation AS (\n"
                "		SELECT n_nationkey, n_name, n_name = 'EGYPT' nation_check\n"
                "		FROM nation),\n"
                " all_nations AS (\n"
                "    select\n"
                "      o_orderyear,\n"
                "      l.l_extendedprice * (1 - l.l_discount) as volume,\n"
                "      n2.n_name as nation,\n"
                "      nation_check\n"
                "    from\n"
                "      part p,\n"
                "       (SELECT * FROM  supplier ORDER BY s_suppkey LIMIT $LIMIT)  s,\n"
                "      (SELECT * FROM  lineitem ORDER BY l_orderkey, l_linenumber LIMIT $LIMIT) l,\n"
                "       (SELECT * FROM  orders ORDER BY o_orderkey LIMIT $LIMIT) o,\n"
                "       (SELECT * FROM  customer ORDER BY c_custkey LIMIT $LIMIT)  c,\n"
                "      nation n1,\n"
                "      supplier_nation n2,\n"
                "      region r\n"
                "    where\n"
                "      p.p_partkey = l.l_partkey\n"
                "      and s.s_suppkey = l.l_suppkey\n"
                "      and l.l_orderkey = o.o_orderkey\n"
                "      and o.o_custkey = c.c_custkey\n"
                "      and c.c_nationkey = n1.n_nationkey\n"
                "      and n1.n_regionkey = r.r_regionkey\n"
                "      and r.r_name = 'MIDDLE EAST'\n"
                "      and s.s_nationkey = n2.n_nationkey\n"
                "      and p.p_type = 'PROMO BRUSHED COPPER'\n"
                "      and o.o_orderdate >= date '1995-01-01'\n"
                "      and o.o_orderdate <= date '1996-12-31') "
                " select\n"
                "  o_orderyear,\n"
                "  sum(case\n"
                "    when nation_check then volume\n"
                "    else 0\n"
                "  end) / sum(volume) as mkt_share\n"
                " from all_nations\n"
                " group by\n"
                "  o_orderyear\n"
                " order by\n"
                "  o_orderyear\n"},
            // using n_name instead of nation alias in tpc-h because aliasing for schema lookups is NYI
            {9, "WITH  yellow_parts AS (\n"
                "         SELECT p_partkey\n"
                "	 FROM part),\n"
                //                "	 WHERE p_name like '%yellow%'),\n"
                "     profit AS (\n"
                "         select\n"
                "    	   n_name,\n"
                "     	    o_orderyear,\n"
                "      	    l.l_extendedprice * (1.0 - l.l_discount) - ps.ps_supplycost * l.l_quantity as amount\n"
                "    	  from\n"
                "    	    yellow_parts p,\n"
                "      	     (SELECT * FROM  supplier ORDER BY s_suppkey LIMIT $LIMIT)  s,\n"
                "      	    (SELECT * FROM  lineitem ORDER BY l_orderkey, l_linenumber LIMIT $LIMIT) l,\n"
                "      	    (SELECT * FROM partsupp ORDER BY ps_partkey, ps_suppkey LIMIT $LIMIT) ps,\n"
                "      	     (SELECT * FROM  orders ORDER BY o_orderkey LIMIT $LIMIT) o,\n"
                "      	    nation n\n"
                "    where\n"
                "      s.s_suppkey = l.l_suppkey\n"
                "      and ps.ps_suppkey = l.l_suppkey\n"
                "      and ps.ps_partkey = l.l_partkey\n"
                "      and p.p_partkey = l.l_partkey\n"
                "      and o.o_orderkey = l.l_orderkey\n"
                "      and s.s_nationkey = n.n_nationkey)\n"
                " select\n"
                "  n_name,\n"
                "  o_orderyear,\n"
                "  sum(amount) as sum_profit\n"
                " from profit\n"
                " group by\n"
                "  n_name,\n"
                "  o_orderyear\n"
                " order by\n"
                "  n_name,\n"
                "  o_orderyear desc\n"},
            {18, "WITH high_quantity AS (\n"
                 " SELECT l_orderkey\n"
                 " FROM (SELECT * FROM  lineitem ORDER BY l_orderkey, l_linenumber LIMIT $LIMIT) l\n"
                 " GROUP BY l_orderkey\n"
                 " HAVING SUM(l_quantity) > 313) \n"
                 " select c.c_name,\n"
                 "    c.c_custkey,\n"
                 "   o.o_orderkey,\n"
                 "   o.o_orderdate,\n"
                 "   o.o_totalprice,\n"
                 "   sum(l.l_quantity)\n"
                 " from\n"
                 "    (SELECT * FROM  customer ORDER BY c_custkey LIMIT $LIMIT)  c,\n"
                 "    (SELECT * FROM  orders ORDER BY o_orderkey LIMIT $LIMIT) o,\n"
                 "   (SELECT * FROM  lineitem ORDER BY l_orderkey, l_linenumber LIMIT $LIMIT) l\n"
                 " where\n"
                 "   o.o_orderkey in (\n"
                 "     select * FROM high_quantity)\n"
                 "   and c.c_custkey = o.o_custkey\n"
                 "   and o.o_orderkey = l.l_orderkey\n"
                 " group by\n"
                 "   c.c_name,\n"
                 "   c.c_custkey,\n"
                 "   o.o_orderkey,\n"
                 "   o.o_orderdate,\n"
                 "   o.o_totalprice\n"
                 " order by\n"
                 "   o.o_totalprice desc,\n"
                 "   o.o_orderdate\n"
                 " limit 100\n"}
    };

}
#endif // TESTING_DEFS_H