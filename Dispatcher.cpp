#include "Dispatcher.h"

Dispatcher::Dispatcher(DBAdapter& db): m_db(db)
{

}



QueryHandler::QueryHandler(DBAdapter& db): m_db(db)
{

}
